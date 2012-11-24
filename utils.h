char *randString(char *s, int n);
char *randNumber(char *s, int n);
char *randAlpha(char *s, int n);

/* error functions from apue */
void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_exit(int error, const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);


