#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char *HEADER = "HTTP/1.0 200 OK";

int
main(int argc, char *argv[])
{
	char buf[BUFSIZ];

	memcpy(buf, HEADER, strlen(HEADER));

	char *token = strtok(buf, " ");
	printf("'%s'\n", token);

	token = strtok(NULL, " ");
	printf("'%s'\n", token);

	token = strtok(NULL, " ");
	printf("'%s'\n", token);

	token = strtok(NULL, " ");
	printf("'%s'\n", token);

	return 0;
}

