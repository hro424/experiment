#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int
main(int argc, char *argv[])
{
	const char *addr = "FE80:0000:0000:0000:1034:5678:ABCD:EF01";
	struct addrinfo *info;
	struct addrinfo hints;
	struct sockaddr_in6 *sock;
	int i;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	getaddrinfo(addr, NULL, &hints, &info);
	sock = (struct sockaddr_in6 *)info->ai_addr;

	for (i = 0; i < 16; i++) {
		printf("%x\n", sock->sin6_addr.s6_addr[i]);
	}

	freeaddrinfo(info);

	return EXIT_SUCCESS;
}
