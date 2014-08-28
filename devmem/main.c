#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

static void
print_help(void)
{
	fprintf(stderr, "usage:\n"
		"\tmd w <addr> <val>\n"
		"\tmd r <addr> <hex bytes>\n");
}

static void
print_hex(const char *buf, int count, unsigned long base)
{
	int i = 0;
	while (count - i >= 16) {
		printf("%.8lX: %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X "
		       "%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n",
		       base, buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3],
		       buf[i + 4], buf[i + 5], buf[i + 6], buf[i + 7],
		       buf[i + 8], buf[i + 9], buf[i + 10], buf[i + 11],
		       buf[i + 12], buf[i + 13], buf[i + 14], buf[i + 15]);

		base += 16;
		i += 16;
	}

	printf("%.8lX: ", base);
	for (; i < count; i++) {
		printf("%.2X ", buf[i]);
	}
	printf("\n");
}

/*
static void
md_read(int fd, unsigned long addr, unsigned long len)
{
	static char buf[BUFSIZ];

	lseek(fd, addr, SEEK_SET);

	while (len >= BUFSIZ) {
		read(fd, buf, BUFSIZ);
		print_hex(buf, BUFSIZ, addr);
		addr += BUFSIZ;
		len -= BUFSIZ;
	}
	read(fd, buf, len);
	print_hex(buf, len, addr);
}
*/

static int
md_write(int fd, unsigned long addr, unsigned long v)
{
	lseek(fd, addr, SEEK_SET);
	return write(fd, &v, sizeof(unsigned long));
}

static void
md_read_map(int fd, unsigned long addr, unsigned long len)
{
	void *mem;

	while (len >= BUFSIZ) {
		mem = mmap(NULL, BUFSIZ, PROT_READ, MAP_SHARED, fd, addr);
		print_hex(mem, BUFSIZ, addr);
		munmap(mem, BUFSIZ);
		addr += BUFSIZ;
		len -= BUFSIZ;
	}

	mem = mmap(NULL, BUFSIZ, PROT_READ, MAP_SHARED, fd, addr);
	print_hex(mem, len, addr);
	munmap(mem, BUFSIZ);
}

int
main(int argc, char *argv[])
{
	int fd;
	int flag;
	unsigned long addr;
	unsigned long v;

	if (argc < 4) {
		print_help();
		return EXIT_FAILURE;
	}

	if (argv[1][0] == 'w') {
		flag = O_WRONLY;
	}
	else if (argv[1][0] == 'r') {
		flag = O_RDONLY;
	}
	else {
		print_help();
		return EXIT_FAILURE;
	}

	addr = strtoul(argv[2], NULL, 16);
	v = strtoul(argv[3], NULL, 16);

	fd = open("/dev/mem", flag);
	if (fd < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	if (flag == O_WRONLY) {
		md_write(fd, addr, v);
	}
	else if (flag == O_RDONLY) {
		md_read_map(fd, addr, v);
	}

	close(fd);

	return EXIT_SUCCESS;
}
