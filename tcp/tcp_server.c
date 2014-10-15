#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


int
main(int argc, char *argv[])
{

	int sock;
	struct addrinfo hints;
	struct addrinfo *result;
	struct sockaddr_in addr;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = PORT;
	
}
