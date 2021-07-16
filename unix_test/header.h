#ifndef HEADER_H
#define HEADER_H

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// fork
#include <unistd.h>

#include <string.h>
#include <stdio.h>

const char *UNIX_FILE = "test_unix_file";

// 服务器收到客户端的ack包完成三次连接，等待accept函数的队列长度，取此值和/proc/sys/net/core/somaxconn的最小值。
const int BACKLOG = 5;

const int MAX_BUF_LEN = 1024;	// buf长度

extern int errno;

#endif
