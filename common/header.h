#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

// select
#include <sys/select.h>
#include <sys/time.h>

// poll
#include <poll.h>

#include <string.h>
#include <stdio.h>

const char *SERVER_IP = "127.0.0.1";
const short SERVER_PORT = 6666;

const int BACKLOG = 5;
const int MAX_BUF_LEN = 1024;		// buf长度

const int SELECT_FD_SET_SIZE = 128;	// select轮询套接字数量

const int OPEN_MAX = 1024;			// poll打开描述符数量

extern int errno;
