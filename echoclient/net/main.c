#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int
_connect(const char *host, uint16_t port)
{
	int fd;
	int err;
	struct addrinfo *info;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	err = getaddrinfo(host, NULL, &hints, &info);

	fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (fd > 0) {
		((struct sockaddr_in *)info->ai_addr)->sin_port = htons(port);
		err = connect(fd, info->ai_addr, info->ai_addrlen);
	}

	freeaddrinfo(info);
	if (err < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

static char msg[] = "Humpty Dumpty sat on a wall, Humpty Dumpty had a great fall.";
static char buf[1024];

int
main(int argc, char *argv[])
{
	int fd;
	int r;

	if (argc < 3) {
		fprintf(stderr, "usage: prog <host> <port>\n");
		return EXIT_FAILURE;
	}

	fd = _connect(argv[1], atoi(argv[2]));
	if (fd < 0) {
		perror("connect");
		return EXIT_FAILURE;
	}

	r = write(fd, msg, strlen(msg));
	if (r < 0) {
		perror("write");
		return EXIT_FAILURE;
	}

	r = read(fd, buf, 1024);
	if (r < 0) {
		perror("read");
		return EXIT_FAILURE;
	}

	printf("%s\n", buf);

	close(fd);
	return EXIT_SUCCESS;
}
