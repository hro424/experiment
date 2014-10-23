#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_IN_PATH	"/tmp/test.soc"
#define MSG1		"hello"
#define MSG2		"event"

static char buf[BUFSIZ];

struct th_arg {
	int soc;
	struct sockaddr_un addr;
	socklen_t addrlen;
};

static void *
worker(void *args)
{
	struct th_arg *p = (struct th_arg *)args;

	for (int i = 0; i < 10; i++) {
		printf("Send to '%s'\n", p->addr.sun_path);
		sendto(p->soc, MSG2, strlen(MSG2) + 1, 0,
		       (struct sockaddr *)&p->addr, p->addrlen);
	}

	return NULL;
}

static pthread_t *
run_event_thread(int soc, struct sockaddr_un *peer, socklen_t addrlen)
{
	pthread_t *th;
	struct th_arg *arg;

	//XXX
	th = malloc(sizeof(pthread_t));
	arg = malloc(sizeof(struct th_arg));

	arg->soc = soc;
	memcpy(&arg->addr, peer, sizeof(struct sockaddr_un));
	arg->addrlen = addrlen;

	pthread_create(th, NULL, worker, arg);

	return th;
}

static int
server(int soc)
{
	ssize_t sz;
	struct sockaddr_un peer;
	socklen_t addrlen;
	pthread_t *th;

	for (;;) {
		sz = recvfrom(soc, buf, BUFSIZ, 0,
			      (struct sockaddr *)&peer, &addrlen);
		if (sz < 0) {
			perror("recvfrom");
			return -1;
		}
		printf("Received from '%s'\n", peer.sun_path);

		int type = buf[0];
		printf("server received (%d)\n", type);

		int len;
		switch (type) {
			case 1:
				len = strlen(MSG1) + 1;
				strcpy(buf, MSG1);
				sz = sendto(soc, buf, len, 0,
					    (struct sockaddr *)&peer, addrlen);
				if (sz < 0) {
					perror("sendto");
					return -1;
				}
				break;
			case 2:
				th = run_event_thread(soc, &peer, addrlen);
				break;
			case 3:
				pthread_cancel(*th);
				return 0;
			default:
				break;
		}
	}
}

int
main(int argc, char *argv[])
{
	int rc;
	int sockfd;
	struct sockaddr_un saddr, saddr2;
	socklen_t len;

	sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	saddr.sun_family = AF_LOCAL;
	memcpy(saddr.sun_path, SERVER_IN_PATH, sizeof(saddr.sun_path) - 1);
	unlink(SERVER_IN_PATH);

	rc = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (rc < 0) {
		perror("bind");
		rc = EXIT_FAILURE;
		goto exit;
	}

	len = sizeof(saddr2);
	getsockname(sockfd, (struct sockaddr*)&saddr2, &len);
	printf("Bound socket to '%s'\n", saddr2.sun_path);

	rc = server(sockfd);
	if (rc < 0) {
		rc = EXIT_FAILURE;
	}
	else {
		rc = EXIT_SUCCESS;
	}

exit:
	close(sockfd);

	return rc;
}
