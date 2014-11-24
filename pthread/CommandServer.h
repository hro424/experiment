#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include "SocketServer.h"
#include "RecognitionEventHandler.h"
#include "DataHandler.h"
#include "RecognitionService.h"

class CommandServer : public SocketServer, RecognitionEventHandler
{
public:
    static const int EVENT_TYPE_BOS = 1;
    static const int EVENT_TYPE_TS = 2;
    static const int EVENT_TYPE_RESULT = 3;
    static const int COMMAND_HELLO = 0;
    static const int COMMAND_START = 1;
    static const int COMMAND_STOP = 2;

    CommandServer(int fd);
    virtual ~CommandServer();
    virtual int HandleRecognitionEvent(int type, unsigned long time);

protected:
    virtual int OnReceive() override;
    int sid;
    RecognitionService* service;
};

CommandServer::CommandServer(int fd)
    : SocketServer(fd)
{
    service = RecognitionService::GetInstance();
    sid = service->Register(this);
}

CommandServer::~CommandServer()
{
    service->Deregister(sid);
}

int
CommandServer::OnReceive()
{
    uint8_t buf[BUFSIZ];
    size_t bufsiz = BUFSIZ;
    ssize_t sz = read(connfd, buf, bufsiz);

    if (buf[0] == COMMAND_HELLO) {
        sz = write(connfd, &sid, sizeof(int));
    }
    else if (buf[0] == COMMAND_START) {
        service->Start(sid);
        //sz = write(connfd, ack, size);
    }
    else if (buf[0] == COMMAND_STOP) {
        service->Stop(sid);
    }

    return 0;
}

int
CommandServer::HandleRecognitionEvent(int type, unsigned long time)
{
    if (type & EVENT_TYPE_BOS) {
    }
    else if (type & EVENT_TYPE_TS) {
    }
    else if (type & EVENT_TYPE_RESULT) {
    }

    return 0;
}

#endif // COMMAND_SERVER_H
