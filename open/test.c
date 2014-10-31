#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int
main()
{
	int fd = open("test.txt", O_WRONLY | O_CREAT, 00644);
	write(fd, "test", 5);
	close(fd);

	return 0;
}

