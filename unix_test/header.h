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

// 客户端已经发起连接 且 还未被服务器accept 的连接总数量
// 未完成连接队列(三次握手还未完成) + 已完成连接队列(三次握手已完成) 总和
const int BACKLOG = 5;

const int MAX_BUF_LEN = 1024;	// buf长度

extern int errno;

#endif
