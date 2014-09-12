#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libwebsockets.h>

static int fd1;
static int fd2;
static char buf[BUFSIZ];

static ssize_t
readline(int fd, void *buf, size_t len)
{
	ssize_t r;
	char c;
	char *cur = buf;

	while (cur < (char *)buf + len) {
		r = read(fd, &c, 1);
		if (r < 0) {
			return r;
		}
		else if (r == 0) {
			break;
		}
		else {
			*cur = c;
			cur++;
			if (c == '\n' || c == '\r') {
				break;
			}
		}
	}

	return cur - (char *)buf;
}

static ssize_t
my_read(int fd, void *buf, size_t len)
{
	ssize_t r;
	ssize_t total = 0;
	char *cur = buf;

	for (;;) {
		r = readline(fd, cur, len);
		if (r < 0) {
			return r;
		}
		else if (r == 0) {
			break;
		}
		else {
			total += r;
			if (strncmp(cur, "EOS", 3) == 0) {
				break;
			}

			cur += r;
			len -= r;
		}
	}

	return total;
}

static int
callback_http(struct libwebsocket_context * this,
	      struct libwebsocket *wsi,
	      enum libwebsocket_callback_reasons reason, void *user,
	      void *in, size_t len)
{
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		printf("connection established\n");
		break;
	case LWS_CALLBACK_RECEIVE: {
		write(fd1, in, len);
		printf("wrote(%zu): %s\n", len, (char *)in);
		len = my_read(fd2, buf, BUFSIZ);
		printf("read(%zu, %zu): %s\n", len, strlen(buf), buf);

		unsigned char *res = malloc(LWS_SEND_BUFFER_PRE_PADDING +
					    len +
					    LWS_SEND_BUFFER_POST_PADDING);

		memcpy(res + LWS_SEND_BUFFER_PRE_PADDING, buf, len);

		libwebsocket_write(wsi, &res[LWS_SEND_BUFFER_PRE_PADDING],
				   len, LWS_WRITE_TEXT);

		free(res);
		break;
	}
	default:
		break;
	}

	return 0;
}

static struct libwebsocket_protocols protocols[] = {
	{
		"http-only",
		callback_http,
		0
	},
	{
		NULL, NULL, 0
	}
};

int
main(int argc, char *argv[]) {
	struct libwebsocket_context *context;
	struct lws_context_creation_info info;

	fd1 = open("/tmp/cabocha.in", O_RDWR);
	if (fd1 < 0) {
		perror("open");
		return EXIT_FAILURE;
	}
	fd2 = open("/tmp/cabocha.out", O_RDWR);
	if (fd2 < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	memset(&info, 0, sizeof(info));
	info.port = 8080;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
	info.protocols = protocols;

	lws_set_log_level(0, NULL);
	context = libwebsocket_create_context(&info);
	if (context == NULL) {
		lwsl_err("libwebsocket init failed\n");
		return EXIT_FAILURE;
	}

	while (1) {
		libwebsocket_service(context, 50);

		// libwebsocket_service will process all waiting events with
		// their callback functions and then wait 50 ms.  (this is a
		// single threaded webserver and this will keep our server
		// from generating load while there are not requests to
		// process)
	}

	libwebsocket_context_destroy(context);
	close(fd1);
	close(fd2);

	return EXIT_SUCCESS;
}
