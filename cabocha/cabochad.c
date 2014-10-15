#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <cabocha.h>

static cabocha_t *cabocha;

static int
callback_http(struct libwebsocket_context *this,
	      struct libwebsocket *wsi,
	      enum libwebsocket_callback_reasons reason,
	      void *user, void *in, size_t len)
{
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		//printf("connected\n");
		break;
	case LWS_CALLBACK_RECEIVE: {
		const char *result = cabocha_sparse_tostr2(cabocha, in, len);
		len = strlen(result);

		unsigned char *res = malloc(LWS_SEND_BUFFER_PRE_PADDING +
					    len +
					    LWS_SEND_BUFFER_POST_PADDING);
		memcpy(res + LWS_SEND_BUFFER_PRE_PADDING, result, len);
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
		.name = "http-only",
		.callback = callback_http,
		.per_session_data_size = 0,
		.rx_buffer_size = 0
	},
	{
		.name = NULL,
		.callback = NULL,
		.per_session_data_size = 0,
		.rx_buffer_size = 0
	}
};

int
main(int argc, char *argv[]) {
	struct libwebsocket_context *context;
	struct lws_context_creation_info info;

	cabocha = cabocha_new(argc, argv);

	memset(&info, 0, sizeof(info));
	info.port = 8080;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
	info.protocols = protocols;

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

	return EXIT_SUCCESS;
}
