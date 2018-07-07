#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// fork
#include <unistd.h>

// select
#include <sys/select.h>
#include <sys/time.h>

// poll
#include <sys/poll.h>

// thread
#include <thread>

// epoll
#include <sys/epoll.h>

#include <string.h>
#include <stdio.h>

const char *SERVER_IP = "127.0.0.1";
const in_port_t SERVER_PORT = 6666;

const int BACKLOG = 5;
const int MAX_BUF_LEN = 1024;	// buf长度

// select/poll/epoll最大描述符数量
const int MAX_FD_NUM = 1024;

extern int errno;
