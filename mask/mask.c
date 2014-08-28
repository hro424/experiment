#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUF_SIZE	0x100
#define BUF_SIZE_MASK	~(BUF_SIZE)
static uint8_t buf[BUF_SIZE] __attribute__((aligned (256)));
static uint8_t *rp;

static inline int
concat(int a, int b) {
	return (((char)b) << 8) | ((char)a & 0xff);
}

static inline int
first(int a) {
	return (char)a;
}

static inline int
last(int a) {
	return a >> 8;
}

static int
inc(void)
{
	int ret;

	ret = *rp;
	rp = (uint8_t *)((uintptr_t)(rp + 1) & BUF_SIZE_MASK);
	return ret;
}

int
main(int argc, char *argv[])
{
	int a = -41;
	int b = -26;
	int c = concat(a, b);

	printf("concat %d\n", c);
	printf("first %d\n", first(c));
	printf("last %d\n", last(c));

	rp = buf;
	for (int i = 0; i < BUF_SIZE + 1; i++) {
		inc();
		printf("%p ", rp);
	}

	return 0;
}
