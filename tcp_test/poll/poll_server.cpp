#include "../common/header.h"

int main()
{
	// 创建套接字
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("scoket error.\n");
		return -1;
	}

	// 服务器套接字
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// 绑定套接字
	if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("bind error.\n");
		close(listen_fd);
		return -1;
	}

	// 监听
	if (listen(listen_fd, BACKLOG) < 0)
	{
		printf("listen error.\n");
		close(listen_fd);
		return -1;
	}

	struct pollfd client_fd[MAX_FD_NUM];
	client_fd[0].fd = listen_fd;
	client_fd[0].events = POLLIN;
	for (int i = 1; i < MAX_FD_NUM; ++i)
	{
		client_fd[i].fd = -1;
	}
	int max_fd_index = 0;	// 最大的可读描述符索引
	char buf[MAX_BUF_LEN] = {0};

	while (true)
	{
		int ready_fd = poll(client_fd, max_fd_index + 1, -1);

		// 监听套接字消息
		if (client_fd[0].revents & POLLIN)
		{
			struct sockaddr_in client_addr;
			socklen_t client_addr_len = sizeof(client_addr);
			int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
			if (connect_fd < 0)
			{
				printf("accept error: continue.\n");
				fflush(stdout);
				continue;
			}
			else
			{
				printf("accept:fd=%d.\n", connect_fd);
				fflush(stdout);

				int i = 0;
				for (; i < MAX_FD_NUM; ++i)
				{
					if (client_fd[i].fd == -1)
					{
						client_fd[i].fd = connect_fd;
						client_fd[i].events = POLLIN;
						break;
					}
				}
				if (i == MAX_FD_NUM)
				{
					printf("too many connection.\n");
					fflush(stdout);
					// 该咋办？
				}
				else
				{
					if (i > max_fd_index)
					{
						max_fd_index = i;
					}
				}
			}
			if (--ready_fd <= 0)
			{
				continue;
			}
		}

		for (int i = 0; i <= max_fd_index; ++i)
		{
			if (client_fd[i].fd < 0)
			{
				continue;
			}
			if (client_fd[i].revents & (POLLIN | POLLERR))
			{
				// 可读事件或错误
				memset(buf, 0, MAX_BUF_LEN);
				ssize_t len = read(client_fd[i].fd, buf, MAX_BUF_LEN);
				if (len > 0)
				{
					printf("recv:: %s\n", buf);

					memset(buf, 0, MAX_BUF_LEN);
					snprintf(buf, MAX_BUF_LEN, "hello client.");
					write(client_fd[i].fd, buf, strlen(buf));
				}
				else if ((len == -1 && errno == EINTR) || errno == EAGAIN)
				{
					// 慢系统调用导致信号中断没有读到数据 或 非阻塞没有数据则返回EAGAIN
					printf("recv error:: continue\n");
				}
				else
				{
					close(client_fd[i].fd);
					printf("connection closed:fd=%d\n", client_fd[i].fd);
					client_fd[i].fd = -1;
				}
				fflush(stdout);
			}
			else if (client_fd[i].revents & POLLOUT)
			{
				// 可写事件
			}
			else
			{
				// 关闭连接
			}
			if (--ready_fd <= 0)
			{
				continue;
			}
		}
	}

	close(listen_fd);
	return 0;
}
