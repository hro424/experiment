#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SOC_PATH	"/tmp/test.soc"

static char buf[BUFSIZ];

int
main(int argc, char *argv[])
{
	int rc, soc;
	struct sockaddr_un client, server, peer;
	socklen_t addrlen;
	char c;

	soc = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (soc < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	client.sun_family = AF_UNIX;
	rc = bind(soc, (struct sockaddr *)&client, sizeof(client));

	server.sun_family = AF_UNIX;
	memcpy(server.sun_path, SOC_PATH, strlen(SOC_PATH) + 1);

	printf("Send msg 1\n");
	c = 1;
	sendto(soc, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));
	recvfrom(soc, buf, BUFSIZ, 0, (struct sockaddr *)&peer, &addrlen);
	printf("%s\n", buf);

	printf("Send msg 2\n");
	c = 2;
	sendto(soc, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));
	for (int i = 0; i < 5; i++) {
		recvfrom(soc, buf, BUFSIZ, 0,
			 (struct sockaddr *)&peer, &addrlen);
		printf("%s\n", buf);
	}

	printf("Send msg 3\n");
	c = 3;
	sendto(soc, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));

	close(soc);

	return EXIT_SUCCESS;
}
