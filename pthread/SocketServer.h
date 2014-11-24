#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <pthread.h>

class SocketServer
{
public:
    SocketServer(int fd);
    virtual int Start();
    virtual void Stop();
    virtual void Join();

protected:
    static void* Run(void* args);
    virtual int OnReceive() = 0;

    int connfd;
    volatile bool isRunning;
    pthread_t server;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

#endif // SOCKET_WORKER_H
