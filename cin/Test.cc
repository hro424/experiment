#include <iostream>
#include <cstdio>
#include <cstdlib>

#define BUFFERSIZE  16000 * 2 * 10
char buffer[BUFFERSIZE];

int
main()
{
    /*
    while (std::cin >> buffer) {
        std::cout << "echo: " << buffer << std::endl;;
    }
    */
    size_t sz;
    while ((sz = fread(buffer, 1, BUFFERSIZE, stdin)) != 0) {
        std::cout << sz << " bytes read" << std::endl;
    }

    return 0;
}
