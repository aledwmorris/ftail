/* ftail.c
 * (C) 2021 Aled Morris
 *
 * intentionally simple implementation of "tail -F" i.e. copy to stdout all text appended to a named file, from the time of invocation.
 * doesn't attempt to print any text already in the file.
 * will re-open the file if the device or inode number changes e.g. a log file is rotated.
 * polls for changes every second.
 * will die gracefully if stdout isn't writeable.
 * unlike tail(1) we can't read multiple files, cope with truncation, or any other fancy features.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>

int
main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}
	for (;;) {
		int fd = open(argv[1], O_RDONLY);
		if (fd < 0) {
			perror(argv[1]);
			exit(1);
		}
		lseek(fd, 0, SEEK_END);
		struct stat sf;
		fstat(fd, &sf);

		for (;;) {
			int n;
			char buf[512];
			bool while_else = true;
			while ((n = read(fd, buf, sizeof(buf))) > 0) {
				if (write(1, buf, n) < 0) exit(0);
				while_else = false;
			}
			if (while_else == true) {
				struct stat sb;
				stat(argv[1], &sb);
 				if (sf.st_dev != sb.st_dev || sf.st_ino != sb.st_ino) break;
			}
			sleep(1);
		}
		close(fd);
	}
}

//end
