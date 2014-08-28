#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static const char *MY_SOCK_PATH = "/tmp/server_sock";
static char buf[BUFSIZ];

int
server_init(void)
{
	int fd;
	struct sockaddr_un my_addr;

	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd == -1) {
		return fd;
	}

	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH,
		sizeof(my_addr.sun_path) - 1);

	if (bind(fd, (struct sockaddr *)&my_addr,
		 sizeof(struct sockaddr_un)) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}

void
server(int fd)
{
	for (;;) {
		int s = read(fd, buf, BUFSIZ);
		if (s < 0) {
			break;
		}
		printf("'%s'\n", buf);
	}
}

int
main(int argc, char *argv[])
{
	server(server_init());

	return 0;
}
