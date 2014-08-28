#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>

#define VMIN_VALUE 1
#define VTIME_VALUE 20

static struct termios orig;
static int fd;


static int
serial_init(int fd)
{
	struct termios term;

	if (tcgetattr(fd, &orig) < 0) {
		return -1;
	}

	cfmakeraw(&term);
	term.c_cflag = CS8 | CLOCAL | CREAD;
	term.c_cc[VMIN] = VMIN_VALUE;
	term.c_cc[VTIME] = VTIME_VALUE;

	if (cfsetispeed(&term, B115200) < 0 ||
	    cfsetospeed(&term, B115200) < 0 ||
	    tcsetattr(fd, TCSANOW, &term) < 0 ||
	    tcflush(fd, TCIOFLUSH) < 0) {
		return -1;
	}

	return 0;
}

int
serial_open(const char *file)
{
	int fd;

       	fd = open(file, O_RDWR | O_NONBLOCK);
	if (fd > 0) {
		if (serial_init(fd) < 0) {
			close(fd);
			fd = -1;
		}
	}

	return fd;
}

static void
fini(int fd)
{
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &orig);
}

void
serial_close(int fd)
{
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &orig);
	close(fd);
}

static void
sigint_action(int signum, siginfo_t *info, void *ctx)
{
	printf("signal: %d\n", info->si_fd);
	serial_close(fd);
	exit(1);
}

int
serial_write(int fd, const void *buf, int len)
{
	struct pollfd fds[1];
	int r = 0;

	fds[0].fd = fd;
	fds[0].events = POLLOUT;

	int left = len;
	while (left > 0) {
		int e = poll(fds, 1, -1);
		if (e > 0) {
			if (fds[0].revents & POLLOUT) {
				int s = write(fd, buf, left);
				left -= s;
				r += s;
			}
			else {
				return e;
			}
		}
		else if (e < 0) {
			return e;
		}
	}
	tcdrain(fd);
	usleep(20000);
	//usleep(40000);

	return r;
}

int
serial_read(int fd, void *buf, int len)
{
	int r = 0;
	struct pollfd fds[1];
	char *b = (char *)buf;

	fds[0].fd = fd;
	fds[0].events = POLLIN;

	int left = len;
	while (left > 0) {
		int e = poll(fds, 1, 500);
		if (e > 0) {
			if (fds[0].revents & POLLIN) {
				int s = read(fd, b + r, left);
				if (s < 0) {
					return s;
				}
				r += s;
				left -= s;
				if (s < len) {
					break;
				}
			}
			else {
				return e;
			}
		}
		else if (e < 0) {
			return e;
		}
		else {
			break;
		}
	}

	return r;
}



// usage: serial <dev>
int
main(int argc, char *argv[])
{
	struct sigaction act;
	int r;
	char buf[BUFSIZ];

	if (argc < 2) {
		fprintf(stderr, "usage: %s <device>\n", argv[0]);
		return EXIT_FAILURE;
	}

	fd = serial_open(argv[1]);

	act.sa_sigaction = sigint_action;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &act, NULL) < 0) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

	static const char *msg = "SKINFO\r\n";
	r = serial_write(fd, msg, strlen(msg));

	r = serial_read(fd, buf, BUFSIZ);
	printf("%s", buf);

	memset(buf, 0, BUFSIZ);
	static const char *msg2 = "SKTABLE 6\r\n";
	r = serial_write(fd, msg2, strlen(msg2));

	do {
		memset(buf, 0, BUFSIZ);
		r = serial_read(fd, buf, BUFSIZ);
		printf("%s", buf);
	} while (r > 0);

exit:
	serial_close(fd);

	return EXIT_SUCCESS;
}

