#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PATH	"/tmp/stream_server.soc"

static void*
handle_request(void* args)
{
	int sockfd = (int)args;
	uint8_t *buf;
	ssize_t sz;

	buf = malloc(BUFSIZ);

	for (;;) {
		sz = recv(sockfd, buf, BUFSIZ, 0);
		if (sz < 0) {
			perror("recv");
			goto exit;
		}
		if (sz == 0) {
			goto exit;
		}

		printf("Received %zd bytes\n", sz);

		ssize_t len = sz;
		switch (buf[0]) {
		case 'a':
			sz = send(sockfd, buf, len, 0);
			if (sz < 0) {
				perror("send");
				goto exit;
			}
			break;
		case 'z':
			goto exit;
		default:
			break;
		}

	}
exit:
	free(buf);
	printf("handler exit\n");
}

static int
server(int sockfd)
{
	int fd;
	socklen_t len;
	struct sockaddr_un addr;
	pthread_t *th;

	for (;;) {
		len = sizeof(addr);
		fd = accept(sockfd, (struct sockaddr *)&addr, &len);
		if (fd < 0) {
			if (errno == EINTR) {
				continue;
			}
			else {
				perror("accept");
				return -1;
			}
		}

		printf("Accepted.\n");
		th = malloc(sizeof(pthread_t));
		pthread_create(th, NULL, handle_request, (void*)fd);
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	int rc;
	int listenfd;
	socklen_t socklen;
	struct sockaddr_un addr, addr2;

	listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (listenfd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SERVER_PATH, sizeof(addr.sun_path) - 1);
	unlink(SERVER_PATH);

	rc = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
	if (rc < 0) {
		perror("bind");
		rc = EXIT_FAILURE;
		goto exit;
	}

	socklen = sizeof(addr2);
	getsockname(listenfd, (struct sockaddr *)&addr2, &socklen);
	printf("Bound socket to '%s'\n", addr2.sun_path);

	rc = listen(listenfd, 16);
	if (rc < 0) {
		perror("listen");
		rc = EXIT_FAILURE;
		goto exit;
	}

	rc = server(listenfd);
	if (rc < 0) {
		rc = EXIT_FAILURE;
	}
	else {
		rc = EXIT_SUCCESS;
	}

exit:
	close(listenfd);
	return rc;
}
