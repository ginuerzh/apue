#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include "../utils.h"

#define	MAX_BUF_LEN	1024 * 1024	// max buffer size: 1MB
#define NULL_DEV	"/dev/null"	// default write data to /dev/null

static const char *infile = NULL, *outfile = NULL_DEV;
static int infd = STDIN_FILENO, outfd; // input and output file descriptors
static int wrdisk;
static int bufSize = MAX_BUF_LEN;
static int bSync;	// write to disk file with O_SYNC set
static int bFsync;	// write to disk followd by fsync
static int bFdatasync; // write to disk followed by fdatasync

static char buf[MAX_BUF_LEN];

static char *truth(int val) {
	if (val)
		return "true";
	else
		return "false";
}

static void set_fl(int fd, int flag) {
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		err_sys("fcntl, F_GETFL error");
	}

	val |= flag;

	if ((val = fcntl(fd, F_SETFL, val)) < 0) {
		err_sys("fcntl, F_SETFL error");
	}
}

void usage() {
	err_quit("Usage: iotest [ -sfd ] [ -B buffer_size ] [ input_file [output_file] ]");
}

void optargs(int argc, char **argv) {
	int opt;

	while ((opt = getopt(argc, argv, "sfdB:")) != -1) {
		switch (opt) {
			case 'B':	// buffer size
				bufSize = atoi(optarg);
				break;	
			case 's':
				bSync = 1;
				break;
			case 'f':
				bFsync = 1;
				break;
			case 'd':
				bFdatasync = 1;
				break;
			default: /* ? */
				usage();
		}
	}
	
	if (bufSize == 0 || bufSize > MAX_BUF_LEN)
		bufSize = MAX_BUF_LEN;

	if (optind < argc)
		infile = argv[optind++];
	if (optind < argc) {
		outfile = argv[optind];
		wrdisk = 1;
	}

	printf("O_SYNC:%s, fsync:%s, fdatasync:%s, buffer_size:%d, input:%s, output:%s.\n",
			truth(bSync), truth(bFsync), truth(bFdatasync), bufSize, infile ? infile : "stdin", outfile);
}

static void transfer(const char *infile, const char *outfile) {
	int count;
	int rcount = 0;
	int wcount = 0;
	int flag = O_WRONLY | O_CREAT;

	//if (wrdisk)
	//	flag |= O_EXCL; // ensure not rewrite exist file
	
	if (infile && (infd = open(infile, O_RDONLY)) < 0) {
		err_sys("open '%s' error", infile);
	}

	assert(outfile != NULL);
	if (outfile && (outfd = open(outfile, flag,  S_IRUSR | S_IWUSR)) < 0) {
		err_sys("open '%s' error", outfile);
	}

	if (bSync)
		set_fl(outfd, O_SYNC);

	while ((count = read(infd, buf, bufSize)) > 0) {
		rcount += count;
		if (count != write(outfd, buf, count)){
			err_ret("write error");
			break;
		}	
		if (bFsync)
			fsync(outfd);

		if (bFdatasync)
			fdatasync(outfd);

		wcount += count;
	}
	if (count < 0)
		err_ret("read error");

	printf("r: %d, w: %d.\n", rcount, wcount);
}

int main(int argc, char **argv) {
	
	optargs(argc, argv);

	transfer(infile, outfile);
	
	exit(EXIT_SUCCESS);
}

