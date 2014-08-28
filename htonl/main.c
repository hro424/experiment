#include <stdio.h>
#include <arpa/inet.h>

int
main(int argc, char *argv[])
{
	uint32_t a = 0x1234abcd;
	uint16_t b = 0x12ab;

	printf("a = 0x%x\n", a);
	printf("htonl(a) = 0x%x\n", htonl(a));
	printf("ntohl(a) = 0x%x\n", ntohl(a));
	printf("b = 0x%x\n", b);
	printf("htons(b) = 0x%x\n", htons(b));
	printf("ntohs(b) = 0x%x\n", ntohs(b));

	return 0;
}
