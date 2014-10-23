#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PATH	"/tmp/stream_server.soc"

static char *message = "aUnix Domain Stream Client/Server";
static char buf[BUFSIZ];

int
main(int argc, char *argv[])
{
	int rc;
	int sockfd;
	struct sockaddr_un addr;

	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SERVER_PATH, sizeof(addr.sun_path) - 1);

	rc = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
		perror("connect");
		return EXIT_FAILURE;
	}

	printf("Sending a type-1 message.\n");
	strcpy(buf, message);
	send(sockfd, &buf, strlen(buf) + 1, 0);
	recv(sockfd, &buf, BUFSIZ, 0);

	close(sockfd);

	return EXIT_SUCCESS;
}

