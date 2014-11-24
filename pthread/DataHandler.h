#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <pthread.h>
#include "SocketServer.h"
#include "Log.h"

class DataHandler : public SocketServer
{
public:
    DataHandler(int fd);
    virtual int OnReceive() override;
};

DataHandler::DataHandler(int fd) : SocketServer(fd)
{
}

int
DataHandler::OnReceive()
{
    LOG_TRACE("ENTER OnReceive");
    LOG_TRACE("EXIT OnReceive");
    return 0;
}

#endif // DATA_HANDLER_H
