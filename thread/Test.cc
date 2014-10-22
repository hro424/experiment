#include <iostream>
#include <thread>
#include <mutex>

namespace {
    char buffer[BUFSIZ];
}

void
input_thread()
{
}

void
consumer_thread()
{
}

int
main()
{
    std::thread th1(input_thread);
    std::thread th2(consumer_thread);

    th1.join();
}
