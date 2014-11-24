#ifndef LOCAL_SERVER_SOCKET_H
#define LOCAL_SERVER_SOCKET_H

#include <cstring>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

template<class T>
class LocalServerSocket
{
public:
    LocalServerSocket<T>(const char* path)
        : backlog(8)
    {
        memset(&serv_addr, 0, sizeof(struct sockaddr_un));
        serv_addr.sun_family = AF_LOCAL;
        strncpy(serv_addr.sun_path, path, sizeof(serv_addr.sun_path) - 1);

        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
    }

    virtual ~LocalServerSocket<T>() {
    }

    int Start();
    void Stop();
    void Join();

private:
    static void* Run(void* args);

    int serv_sockfd;
    struct sockaddr_un serv_addr;
    int backlog;
    pthread_t serv_thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool isRunning;
};

template<class T>
int
LocalServerSocket<T>::Start()
{
    if (!isRunning) {
        serv_sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
        unlink(serv_addr.sun_path);

        if (pthread_create(&serv_thread, NULL, Run, this) != 0) {
            return -1;
        }

        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

template<class T>
void
LocalServerSocket<T>::Stop()
{
    if (isRunning) {
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        connect(fd, reinterpret_cast<struct sockaddr*>(&serv_addr),
                sizeof(struct sockaddr_un));
    }
}

template<class T>
void
LocalServerSocket<T>::Join()
{
    if (isRunning) {
        pthread_join(serv_thread, nullptr);
    }
}

template<class T>
void*
LocalServerSocket<T>::Run(void* args)
{
    LocalServerSocket* obj = static_cast<LocalServerSocket*>(args);
    if (bind(obj->serv_sockfd, (struct sockaddr*)&obj->serv_addr,
             sizeof(struct sockaddr_un)) != 0) {
    }

    listen(obj->serv_sockfd, obj->backlog);
    obj->isRunning = true;
    while (obj->isRunning) {
        struct sockaddr_un addr;
        socklen_t addr_len;
        int connfd;

        if ((connfd = accept(obj->serv_sockfd,
                             reinterpret_cast<struct sockaddr*>(&addr),
                             &addr_len)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                break;
            }
        }

        T* server = new T(connfd);
        server->Start();
        //list_append(obj->server_list, server);
    }

    /*
    list_for_each(server_list) {
        it.Stop();
    }
    list_for_each(server_list) {
        it.Join();
    }
    list_for_each(server_list) {
        delete it;
    }
    */

    return NULL;
}

#endif // LOCAL_SERVER_SOCKET_H
