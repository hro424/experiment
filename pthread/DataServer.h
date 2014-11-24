#ifndef DATA_SERVER
#define DATA_SERVER

class DataServer : public SocketServer
{
public:
    DataServer();

protected:
    virtual int HandleEvent() override;
};

DataServer::DataServer()
{
    service = RecognitionService::GetInstance();
}

int
DataServer::HandleEvent()
{
    ssize_t sz = read(connfd, buf, bufsiz);

    printf("%zd bytes read\n", sz);

    service.Process(header.sid, buf + sizeof(header), header.length);
}

#endif // DATA_SERVER
