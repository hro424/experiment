#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

static struct timeval
do_eval(uint32_t x, int t, int (*func)(uint32_t))
{
	struct timeval time1, time2, delta;
	volatile int v;

	// warm up
	v = func(x);

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

static void
eval(const char* filename, int (*func)(uint32_t))
{
	uint32_t x;
	const int loop = 100000;
	struct timeval delta;
	FILE *fp;

	fp = fopen(filename, "w");

	for (int i = 0; i < 32; i++) {
		x = 1U << i;
		delta = do_eval(x, loop, func);

		fprintf(fp, "%.8X %ld\n",
		        x,
			delta.tv_sec * 1000000 + delta.tv_usec);
	}

	fclose(fp);
}

#ifdef DEBUG
static void
dump(int (*func)(uint32_t))
{
	uint32_t x;
	for (int i = 0; i < 32; i++) {
		x = 1U << i;
		printf("%x: %d\n", x, func(x));
	}
}
#endif

extern int nlz1(uint32_t x);
extern int nlz2(uint32_t x);
extern int nlz3(uint32_t x);
extern int nlz4(uint32_t x);

int
main(int argc, char *argv[])
{
#ifdef DEBUG
	dump(nlz1);
	dump(nlz2);
	dump(nlz3);
#endif
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(2, &cpuset);
	sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

	eval("nlz1.dat", nlz1);
	eval("nlz2.dat", nlz2);
	eval("nlz3.dat", nlz3);
	eval("nlz4.dat", nlz4);

	return EXIT_SUCCESS;
}

