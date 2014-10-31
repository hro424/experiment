#include <iostream>
#include <vector>

class BaseEventHandler
{
public:
    virtual void HandleEvent(int evt) = 0;
};

class EventHandlerImpl : public BaseEventHandler
{
public:
    virtual void HandleEvent(int evt) {
        std::cout << "Event: " << evt << std::endl;
    }
};

class EventSource
{
public:
    void AddEventHandler(BaseEventHandler* h) {
        handlers.push_back(h);
    }

    void Process(int n) {
        for (auto h : handlers) {
            h->HandleEvent(n);
        }
    }

private:
    std::vector<BaseEventHandler*> handlers;
};


int
main()
{
    EventSource src;
    EventHandlerImpl eh1;
    EventHandlerImpl eh2;
    EventHandlerImpl eh3;

    src.AddEventHandler(&eh1);
    src.AddEventHandler(&eh2);
    src.AddEventHandler(&eh3);

    src.Process(1);
    src.Process(2);
    src.Process(3);

    return 0;
}
