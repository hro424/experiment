#include <stdio.h>
#include <stdint.h>


int
main(int argc, char *argv[])
{
	uint32_t a32[4] =
		{ 0xfe800000, 0x00000000, 0x10345678, 0xabcdef01 };
	uint16_t *a16 = (uint16_t *)a32;
	uint8_t *a8 = (uint8_t *)a32;
	int i;

	printf("%X %X %X %X\n", a32[0], a32[1], a32[2], a32[3]);

	for (i = 0; i < 8; i++) {
		printf("%X ", a16[i]);
	}
	printf("\n");

	for (i = 0; i < 16; i++) {
		printf("%x ", a8[i]);
	}
	printf("\n");

	return 0;
}

