#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int flag;

	if (argc < 2){
		printf("Usage: fcntl <descriptor#>\n");	
		exit(1);
	}

	if ((flag = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
		perror("fcntl");
		exit(1);
	}
				
	switch(flag & O_ACCMODE) {
		case O_RDONLY:
			printf("read only.");
			break;
		case O_WRONLY:
			printf("write only.");
			break;
		case O_RDWR:
			printf("read write.");
			break;
		default:
			printf("unknown.");
	}

	if (flag & O_APPEND)
		printf(" append");
	if (flag & O_NONBLOCK)
		printf(" nonblock");
	if (flag & O_SYNC)
		printf(" sync");
	if (flag & O_FSYNC)
		printf(" fsync");

	printf("\n");
	return 0;
}
