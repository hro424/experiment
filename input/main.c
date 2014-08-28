#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static const char *PATH = "/dev/input/event0";
int
main(int argc, char *argv[])
{
	int fd = open(PATH, O_RDONLY);

	if (fd < 0) {
		perror("open1");
		return EXIT_FAILURE;
	}
	printf("fd = %d\n", fd);

	fd = open(PATH, O_RDONLY);
	if (fd < 0) {
		perror("open2");
		return EXIT_FAILURE;
	}
	printf("fd = %d\n", fd);

	close(fd);
	return EXIT_SUCCESS;
}
