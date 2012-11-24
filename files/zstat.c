#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "../utils.h"

int verbose;

void usage() {
	err_quit("Usage: zstat [ -v ] ...file_list");
}

int optargs(int argc, char **argv) {
	int opt;

	while ((opt = getopt(argc, argv, "v")) != -1) {
		switch (opt) {
			case 'v':
				verbose = 1;
				break;
			default:
				usage();
		}
	}

	return optind;
}

int main(int argc, char **argv) {
	struct stat st;
	int index;

	index = optargs(argc, argv);
	if (index >= argc)
		exit(EXIT_SUCCESS);

	argv += (index - 1);

	while (*++argv) {
		if (lstat(*argv, &st) < 0) {
			err_ret("stat '%s' error", *argv);
			continue;
		}

		printf("%s: ", *argv);

		if (S_ISREG(st.st_mode))
			printf("regular file");
		else if (S_ISDIR(st.st_mode))
			printf("directory");
		else if (S_ISCHR(st.st_mode))
			printf("character special file");
		else if (S_ISBLK(st.st_mode))
			printf("block special file");
		else if (S_ISFIFO(st.st_mode))
			printf("pipe");
		else if (S_ISLNK(st.st_mode))
			printf("symbolic link");
		else if (S_ISSOCK(st.st_mode))
			printf("socket");
		else
			printf("unknown file type");

		if (S_ISUID & st.st_mode)
			printf(", set-uid");
		if (S_ISGID & st.st_mode)
			printf(", set-gid");
		printf("\n");

		if (verbose)
			printf("i-node number: %d, device number: %d|%d\n"
					"links: %d, uid: %d, gid: %d\n"
					"size: %d, block size: %d, blocks: %d\n"
					"atime: %smtime: %sctime: %s\n",
					(int)st.st_ino, (int)st.st_dev, (int)st.st_rdev,
					(int)st.st_nlink, (int)st.st_uid, (int)st.st_gid,
					(int)st.st_size, (int)st.st_blksize, (int)st.st_blocks,
					ctime(&st.st_atime), ctime(&st.st_mtime), ctime(&st.st_ctime));

	}

	exit(EXIT_SUCCESS);
}
