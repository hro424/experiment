#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define PAGE_SHIFT	12
#define PAGE_SIZE	(1 << PAGE_SHIFT)
#define PAGE_MASK	(PAGE_SIZE - 1)

static void
mem_read(int fd, const char *addr_str)
{
	unsigned long addr = strtoul(addr_str, NULL, 16);
	printf("read %s 0x%.8lX\n", addr_str, addr);
	unsigned long *base = mmap(NULL, PAGE_SIZE, PROT_READ, MAP_SHARED, fd,
				   addr >> PAGE_SHIFT);
	unsigned long offset = (addr & PAGE_MASK) / sizeof(unsigned long);

	if (base == MAP_FAILED) {
		perror("mmap");
		return;
	}

	printf("%s: 0x%.8lX\n", addr_str, *(base + offset));

	munmap(base, PAGE_SIZE);
}

static void
mem_write(int fd, const char *addr_str, const char *val_str)
{
	unsigned long addr = strtoul(addr_str, NULL, 16);
	unsigned long val = strtoul(val_str, NULL, 16);
	unsigned long *base = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
				   addr >> PAGE_SHIFT);
	unsigned long offset = (addr & PAGE_MASK) / sizeof(unsigned long);

	if (base == MAP_FAILED) {
		perror("mmap");
		return;
	}

	printf("%s: 0x%.8lX\n", addr_str, *(base + offset));
	*base = val;
	printf("%s: 0x%.8lX\n", addr_str, *(base + offset));
	munmap(base, PAGE_SIZE);
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "xm [w|r] addr <val>\n");
		return EXIT_FAILURE;
	}

	int fd = open("/dev/kmem", O_RDWR);
	if (fd < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	if (strncmp(argv[1], "r", 1) == 0) {
		mem_read(fd, argv[2]);
	}
	else if (strncmp(argv[1], "w", 1) == 0) {
		mem_write(fd, argv[2], argv[3]);
	}

	close(fd);

	return EXIT_SUCCESS;
}
