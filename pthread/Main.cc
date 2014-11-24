#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <signal.h>
#include "LocalServerSocket.h"
#include "CommandServer.h"
#include "DataHandler.h"

namespace {

const char* CSOCKPATH = "/tmp/com.sock";
const char* DSOCKPATH = "/tmp/dat.sock";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void
sig_handler(int sig)
{
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

} // namespace

int
main()
{
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "Warning: Failed to set signal handler.\n");
    }

    LocalServerSocket<CommandServer> csock(CSOCKPATH);
    LocalServerSocket<DataHandler> dsock(DSOCKPATH);

    csock.Start();
    dsock.Start();

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    csock.Stop();
    dsock.Stop();

    csock.Join();
    dsock.Join();

    return EXIT_SUCCESS;
}
