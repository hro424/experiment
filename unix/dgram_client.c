#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_SOCK_PATH	"/tmp/test.soc"
//#define TEST
#define CLIENT_SOCK_PATH	"/tmp/test.cli.soc"

static char buf[BUFSIZ];

int
main(int argc, char *argv[])
{
	int rc, sockfd;
	struct sockaddr_un client, server, peer, info;
	socklen_t addrlen;
	char c;

	sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	memset(&client, 0, sizeof(struct sockaddr_un));
	client.sun_family = AF_LOCAL;
#ifdef TEST
	strncpy(client.sun_path, CLIENT_SOCK_PATH,
		sizeof(client.sun_path) - 1);
	unlink(CLIENT_SOCK_PATH);
#endif

	rc = bind(sockfd, (struct sockaddr *)&client, sizeof(client));

	addrlen = sizeof(struct sockaddr_un);
	getsockname(sockfd, (struct sockaddr *)&info, &addrlen);
	printf("Bound socket to '%s'\n", info.sun_path);

	memset(&server, 0, sizeof(struct sockaddr_un));
	server.sun_family = AF_LOCAL;
	strncpy(server.sun_path, SERVER_SOCK_PATH,
		sizeof(server.sun_path) - 1);

	for (int i = 0; i < 2; i++) {
	printf("Sending a type-1 message.\n");
	c = 1;
	sendto(sockfd, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));
	recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr *)&peer, &addrlen);
	printf("Server response @ %s: '%s'\n", peer.sun_path, buf);
	}

	printf("Send type-2 messages.\n");
	c = 2;
	sendto(sockfd, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));
	for (int i = 0; i < 5; i++) {
		recvfrom(sockfd, buf, BUFSIZ, 0,
			 (struct sockaddr *)&peer, &addrlen);
		printf("Server reponse @ %s: '%s'\n", peer.sun_path, buf);
	}

	printf("Send a type-3 message.\n");
	c = 3;
	sendto(sockfd, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));

	close(sockfd);

	return EXIT_SUCCESS;
}
