#ifndef RECOGNITION_SERVICE_H
#define RECOGNITION_SERVICE_H

#include "RecognitionEventHandler.h"

class RecognitionService
{
public:
    static RecognitionService* GetInstance();

    int Register(RecognitionEventHandler* handler);
    void Deregister(int sid);

    int Start(int sid);
    int Stop(int sid);
};

RecognitionService*
RecognitionService::GetInstance()
{
    return nullptr;
}

int
RecognitionService::Register(RecognitionEventHandler* handler)
{
    return 0;
}

void
RecognitionService::Deregister(int sid)
{

}

int
RecognitionService::Start(int sid)
{
    return 0;
}

int
RecognitionService::Stop(int sid)
{
    return 0;
}

#endif // RECOGNITION_SERVICE_H
