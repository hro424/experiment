#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SOC_PATH	"/tmp/test.soc"
#define MSG1		"hello"
#define MSG2		"event"

static char buf[BUFSIZ];

struct th_arg {
	int soc;
	struct sockaddr_un addr;
	socklen_t addrlen;
};

struct th_arg th_args;

static void *
worker(void *args)
{
	for (int i = 0; i < 10; i++) {
		sendto(th_args.soc, MSG2, strlen(MSG2) + 1, 0,
		       (struct sockaddr *)&th_args.addr, th_args.addrlen);
		sleep(1);
	}

	return NULL;
}

static pthread_t *
run_event_thread(int soc, struct sockaddr_un *peer, socklen_t addrlen)
{
	pthread_t *th;

	th = malloc(sizeof(pthread_t));

	th_args.soc = soc;
	memcpy(&th_args.addr, peer, sizeof(struct sockaddr_un));
	th_args.addrlen = addrlen;

	pthread_create(th, NULL, worker, NULL);

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
	int soc;
	struct sockaddr_un soc_addr;

	soc = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (soc < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	soc_addr.sun_family = AF_UNIX;
	memcpy(soc_addr.sun_path, SOC_PATH, strlen(SOC_PATH) + 1);

	rc = bind(soc, (struct sockaddr *)&soc_addr, sizeof(soc_addr));
	if (rc < 0) {
		perror("bind");
		rc = EXIT_FAILURE;
		goto exit;
	}

	rc = server(soc);
	if (rc < 0) {
		rc = EXIT_FAILURE;
	}
	else {
		rc = EXIT_SUCCESS;
	}

exit:
	close(soc);
	//unlink(SOC_PATH);

	return rc;
}
