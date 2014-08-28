#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int
func(uint8_t p)
{
	return p + 2;
}

int
main(int argc, char *argv[])
{
	char a;
	uint8_t b;
	int c;

	c = 254;
	a = (char)c;
	printf("(int)%d (char)%d\n", c, a);

	c = 255;
	a = (char)c;
	printf("(int)%d (char)%d\n", c, a);

	c = 256;
	a = (char)c;
	printf("(int)%d (char)%d\n", c, a);

	b = 254;
	a = b;
	printf("(uint8_t)%u (char)%d\n", b, a);

	b = 255;
	a = b;
	printf("(uint8_t)%u (char)%d\n", b, a);

	printf("%d\n", func(255));

	return 0;
}
