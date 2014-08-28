#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static const char *SERVER_SOCK_PATH = "/tmp/server_sock";
//static char buf[BUFSIZ];

int
client_init(void)
{
	return socket(AF_UNIX, SOCK_DGRAM, 0);
}

void
client(int fd)
{
	int r;
	struct sockaddr_un addr;

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SERVER_SOCK_PATH,
		sizeof(addr.sun_path) - 1);

	for (int i = 0; i < 5; i++) {
		printf("send\n");
		r = sendto(fd, "Hello, world!", 14, 0,
			   (struct sockaddr *)&addr, sizeof(addr));
		if (r < 0) {
			perror("send");
			break;
		}
	}
}

int
main(int argc, char *argv[])
{
	int fd;

	if ((fd = client_init()) < 0) {
		perror("client_init");
		return 1;
	}

	client(fd);

	close(fd);

	return 0;
}
