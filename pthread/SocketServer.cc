#include <cstdlib>
#include <poll.h>
#include <pthread.h>
#include "SocketServer.h"

SocketServer::SocketServer(int fd)
    : connfd(fd), isRunning(false)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

int
SocketServer::Start()
{
    pthread_mutex_lock(&mutex);

    if (isRunning) {
        return 0;
    }

    if (pthread_create(&server, NULL, Run, this) != 0) {
        return -1;
    }

    pthread_cond_wait(&cond, &mutex);

    isRunning = true;

    pthread_mutex_unlock(&mutex);

    return 0;
}

void
SocketServer::Stop()
{
    if (isRunning) {
        isRunning = false;
    }
}

void
SocketServer::Join()
{
    pthread_join(server, NULL);
}

void*
SocketServer::Run(void* args)
{
    SocketServer* ss = static_cast<SocketServer*>(args);
    int rc;
    struct pollfd fds[1];

    fds[0].fd = ss->connfd;
    fds[0].events = POLLIN;

    pthread_mutex_lock(&ss->mutex);
    pthread_cond_broadcast(&ss->cond);
    pthread_mutex_unlock(&ss->mutex);

    while (ss->isRunning) {
        rc = poll(fds, 1, 500);
        if (rc < 0) {
        }
        else if (rc == 0) {
            continue;
        }

        if (fds[0].revents & POLLIN) {
            if (ss->OnReceive() < 0) {
                ss->isRunning = false;
                break;
            }
        }
    }

    return NULL;
}
