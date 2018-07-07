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

	int client_fd[MAX_FD_NUM] = {0};
	for (int i = 0; i < MAX_FD_NUM; ++i)
	{
		client_fd[i] = -1;
	}
	fd_set all_rset;
	FD_ZERO(&all_rset);
	FD_SET(listen_fd, &all_rset);
	fd_set all_wset;
	fd_set all_eset;
	int max_fd = listen_fd;	// 待测试的最大描述符
	int max_fd_index = -1;	// 最大的可读/可写描述符索引
	char buf[MAX_BUF_LEN] = {0};

	while (true)
	{
		fd_set rset = all_rset;
		fd_set wset = all_wset;
		fd_set eset = all_eset;
		int ready_fd = select(max_fd + 1, &rset, &wset, &eset, nullptr);

		// 监听套接字消息
		if (FD_ISSET(listen_fd, &rset))
		{
			struct sockaddr_in client_addr;
			socklen_t client_addr_len = sizeof(client_addr);
			int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
			if (connect_fd < 0)
			{
				printf("accept error: continue.\n");
				fflush(stdout);
			}
			else
			{
				printf("accept:fd=%d.\n", connect_fd);
				fflush(stdout);

				int i = 0;
				for (; i < MAX_FD_NUM; ++i)
				{
					if (client_fd[i] == -1)
					{
						client_fd[i] = connect_fd;
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
					FD_SET(connect_fd, &all_rset);
					FD_SET(connect_fd, &all_wset);
					FD_SET(connect_fd, &all_eset);
					if (connect_fd > max_fd)
					{
						max_fd = connect_fd;
					}
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

		// 可读套接字消息
		for (int i = 0; i <= max_fd_index; ++i)
		{
			if (client_fd[i] < 0)
			{
				continue;
			}

			// 可读事件
			if (FD_ISSET(client_fd[i], &rset))
			{
				memset(buf, 0, MAX_BUF_LEN);
				ssize_t len = read(client_fd[i], buf, MAX_BUF_LEN);
				if (len > 0)
				{
					printf("recv:: %s\n", buf);

					memset(buf, 0, MAX_BUF_LEN);
					snprintf(buf, MAX_BUF_LEN, "hello client.");
					write(client_fd[i], buf, strlen(buf));
				}
				else if ((len == -1 && errno == EINTR) || errno == EAGAIN)
				{
					// 慢系统调用导致信号中断没有读到数据 或 非阻塞没有数据则返回EAGAIN
					printf("recv error:: continue\n");
				}
				else
				{
					FD_CLR(client_fd[i], &all_rset);
					FD_CLR(client_fd[i], &all_wset);
					FD_CLR(client_fd[i], &all_eset);
					close(client_fd[i]);
					printf("connection closed:fd=%d\n", client_fd[i]);
					client_fd[i] = -1;
				}
				fflush(stdout);
			}

			// 可写事件
			if (FD_ISSET(client_fd[i], &wset))
			{
				;
			}

			// 错误事件
			if (FD_ISSET(client_fd[i], &eset))
			{
				;
			}

			// 提前终止for循环
			if (--ready_fd <= 0)
			{
				break;
			}
		}
	}

	close(listen_fd);
	return 0;
}
