#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <stdio.h>

const char *SERVER_IP = "127.0.0.1";
const short SERVER_PORT = 6666;

const int BACKLOG = 5;
const int MAX_BUF_LEN = 1024;		// buf长度

extern int errno;
