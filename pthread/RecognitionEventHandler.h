#ifndef RECOGNITION_EVENT_HANDLER_H
#define RECOGNITION_EVENT_HANDLER_H

class RecognitionEventHandler
{
public:
    virtual int HandleRecognitionEvent(int type, unsigned long time) = 0;
};

#endif // RECOGNITION_EVENT_HANDLER_H
