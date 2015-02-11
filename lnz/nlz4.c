#include <stdint.h>
	
int
nlz4(uint32_t x)
{
	int n = 0;
	int32_t y = (int32_t)x;
	int32_t z = (int32_t)x;
retry:
	if (y < 0) return n;
	if (z == 0) return 32 - n;

	n++;
	y <<= 1;
	z >>= 1;
	goto retry;
}

