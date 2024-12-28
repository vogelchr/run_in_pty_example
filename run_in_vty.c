#define _XOPEN_SOURCE 1 /* for getpt, grantpt, unlockpt... */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int ptmx;
	int pts, i;
	char pts_fn[64];
	pid_t slave;

	if ((ptmx = open("/dev/ptmx", O_RDWR)) == -1) {
		perror("/dev/ptmx");
		exit(1);
	}

	if (ioctl(ptmx, TIOCGPTN, &i) == -1) {
		perror("ioctl(TIOCGPTN)");
		exit(1);
	}

	if (ioctl(ptmx, TIOCGPTN, &i) == -1) {
		perror("ioctl(TIOCGPTN)");
		exit(1);
	}

	snprintf(pts_fn, sizeof(pts_fn), "/dev/pts/%d", i);

	i = 0;
	if (ioctl(ptmx, TIOCSPTLCK, &i) == -1) {
		perror("ioctl(TIOCGPTN)");
		exit(1);
	}

	if ((pts = open(pts_fn, O_RDWR)) == -1) {
		perror(pts_fn);
		exit(1);
	}

	if ((slave = fork()) == 0) {
		/* in slave */
		close(0); /* STDIN */
		close(1); /* STDOUT */
		close(2); /* STDERR */
		if ((dup2(pts, 0) == -1) || (dup2(pts, 1) == -1) ||
		    (dup2(pts, 2) == -1)) {
			exit(1);
		}
		close(ptmx);
		close(pts);

		execl(argv[1], argv[1], NULL);;
	}

	while (1) {
		char buf[128];
		ssize_t r,t;
		
		r = read(ptmx, buf, sizeof(buf));
		if (r == 0) {
			fprintf(stderr, "eof reached.\n");
			exit(0); /* eof */
		}
		if (r == -1) {
			perror("/dev/ptmx");
			exit(1);
		}

		/* write buffer */
		t = write(1, buf, r);
		if (t != r ) {
			fprintf(stderr, "cannot write to stdout.\n");
			exit(1);
		}
	}

}
