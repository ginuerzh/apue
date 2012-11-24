#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define	BUFF_LEN 10

int main() {
	int fd, count;
	char rbuf[BUFF_LEN];
	char wbuf[] = "abcdefghijklmnopqrstuvwxyz";

	memset(rbuf, 0, BUFF_LEN);

	if ((fd = open("stdout", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
		perror("open");
		exit(0);
	}
/*
	if (dup2(fd, STDOUT_FILENO) < 0) {
		perror("dup2");
	}
*/
	if ((count = write(fd, wbuf, sizeof(wbuf))) < 0)
		perror("write");

	printf("write %d data\n", count);

	memset(rbuf, 0, sizeof(BUFF_LEN));
	if ((count = read(fd, rbuf, BUFF_LEN)) < 0)
		perror("read");
	printf("read %d data: %s\n", count, rbuf);

	return 0;
}
