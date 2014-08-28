#include <stdio.h>
#include <stdbool.h>

int
main(int argc, char *argv[])
{
	bool var = true;

	printf("test: %d\n", var);

	for (int i = 0; i < 10; i++) {
		printf("hello\n");
	}

	return 0;
}
