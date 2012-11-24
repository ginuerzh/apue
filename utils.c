#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

#define	MAX_LENGTH 16
#define	MAXLINE 1024

static char *randSeq(const char *charset, char *s, int n);

char *randString(char *s, int n) {
	char *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	return randSeq(charset, s, n);
}

char *randNumber(char *s, int n) {
	char *charset = "0123456789";
	
	return randSeq(charset, s, n);
}

char *randAlpha(char *s, int n) {
	char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	return randSeq(charset, s, n);
}

char *randSeq(const char *charset, char *s, int n) {
	char *p = s;
	int count = n, len;

	if (p && charset) {
		srandom(time(NULL));
		len = strlen(charset);

		while (count--)
			*p++ = *(charset + random() % len);		
	}

	return s;
}

/* 
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
	char buf[MAXLINE];

	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag)
		snprintf(buf + strlen(buf), MAXLINE - strlen(buf), 
				": %s", strerror(error));
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void err_ret(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...) {
	va_list ap;
	
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and terminate.
 */
void err_exit(int error, const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void err_dump(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	abort(); 	/* dump core and terminate */
	exit(EXIT_FAILURE);	/* shouldn't get here */
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_msg(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

int main1(int argc, char **argv) {
	char *s;
	int len = MAX_LENGTH + 1;

	s = (char *)malloc(len);
	printf("%s\n", randString(s, len));
	printf("%s\n", randNumber(s, len));
	printf("%s\n", randAlpha(s, len));	
	free(s);
	
	return 0;
}

