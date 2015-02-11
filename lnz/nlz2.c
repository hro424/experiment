#include <stdint.h>
	
int
lnz2(uint32_t x)
{
	if (x == 0) {
		return 32;
	}

	int n = 0;
	while (!(x & 0x80000000)) {
		x <<= 1;
		n++;
	}
	return n;
}

