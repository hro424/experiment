#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define NAMELEN	255

static char host[NAMELEN];
static char user[NAMELEN];
static char passwd[NAMELEN];
static uint16_t port;

static void
read_proxy_setting(void)
{
	char *p, *q;

	p = getenv("https_proxy");
	p = strstr(p, "://");
	if (!p) {
		return;
	}

	p += 3;
	q = strchr(p, '@');
	if (q) {
		char *r = strchr(p, ':');
		if (r) {
			strncpy(user, p, r - p);
			strncpy(passwd, r + 1, q - r - 1);
		}
		else {
			strncpy(user, p, q - p);
		}
	}
	else {
		q = p;
	}

	q = strchr(p, ':');
	if (q) {
		strncpy(host, p, q - p);
		port = (uint16_t)atoi(q + 1);
	}
	else {
		strncpy(host, p, NAMELEN);
	}
}

int
main(int argc, char *argv)
{
	read_proxy_setting();

	printf("user %s\n", user);
	printf("passwd %s\n", passwd);
	printf("host %s\n", host);
	printf("port %u\n", port);

	return 0;
}

