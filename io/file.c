#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include "../utils.h"

#define FILE_NAME_LEN	14

int main() {
	int fd, count;
	off_t offset;
	char fname[FILE_NAME_LEN + 1];
	char wbuff[] = "abcdefghi";
	char rbuff[10];
	char *fname2 = "abcdefg";

	memset(fname, 0, sizeof(fname));
#ifdef NAME_MAX
	printf("NAME_MAX: %d\n", NAME_MAX);
#endif
	

	randString(fname, FILE_NAME_LEN);
	printf("filename length:%d\n", (int)strlen(fname));

	if ((fd = open(fname2, O_WRONLY | O_CREAT)) < 0) {
		perror("open");
		exit(1);
	}
	printf("fd=%d\n", fd);
	
	//set offset to file end
	if ((offset = lseek(fd, 0, SEEK_END)) == -1) {
		perror("lseek");
		exit(1);
	}
	printf("current offset: %d\n", (int)offset);
	
	if ((count = write(fd, wbuff, sizeof(wbuff))) < 0) {
		perror("write");
	} else {
		printf("write %d data\n", count);
	}

/*
	if ((count = read(fd, rbuff, sizeof(rbuff))) < 0) {
		perror("read");
	} else {
		printf("read %d data\n", count);
	}
*/	
	if ((offset = lseek(fd, 0, SEEK_CUR)) == -1) {
		perror("lseek");
		exit(1);
	}
	printf("current offset: %d\n", (int)offset);

	return 0;
}
