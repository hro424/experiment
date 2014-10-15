#include <iostream>
#include <string.h>

int
main()
{
    char *null_string = nullptr;
    size_t len;

    len = strlen(null_string);

    printf("%zd\n", len);

    return 0;
}
