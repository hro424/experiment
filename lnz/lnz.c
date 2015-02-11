#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

extern int lnz1(uint32_t x);
extern int lnz2(uint32_t x);

struct timeval
do_test(uint32_t x, int t, int (*func)(uint32_t))
{
	struct timeval time1, time2, delta;
	volatile int v;

	gettimeofday(&time1, NULL);
	for (int i = 0; i < t; i++) {
		v += func(x);
	}
	gettimeofday(&time2, NULL);

	if (time1.tv_usec > time2.tv_usec) {
		delta.tv_usec = 1000000 - time1.tv_usec + time2.tv_usec;
	}
	else {
		delta.tv_usec = time2.tv_usec - time1.tv_usec;
	}
	delta.tv_sec = time2.tv_sec - time1.tv_sec;

	return delta;
}

void
test(const char* filename, int (*func)(uint32_t))
{
	uint32_t x;
	const int loop = 10000;
	struct timeval delta;
	FILE *fp;

	fp = fopen(filename, "w");

	for (int i = 0; i < 32; i++) {
		x = 1U << i;
		delta = do_test(x, loop, func);

		fprintf(fp, "%.8X %ld\n",
		        x,
			delta.tv_sec * 1000000 + delta.tv_usec);
	}

	fclose(fp);
}

int
main(int argc, char *argv[])
{
	test("lnz1.dat", lnz1);
	test("lnz2.dat", lnz2);

	return EXIT_SUCCESS;
}

