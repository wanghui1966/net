#include "../common/header.h"

int main()
{
	// 创建套接字
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("scoket error.\n");
		return 1;
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
		return 1;
	}

	// 监听
	if (listen(listen_fd, BACKLOG) < 0)
	{
		printf("listen error.\n");
		return -1;
	}

	struct pollfd client_fd[OPEN_MAX];
	client_fd[0].fd = listen_fd;
	client_fd[0].events = POLLIN;
	for (int i = 1; i < OPEN_MAX; ++i)
	{
		client_fd[i].fd = -1;
	}
	int max_read_fd_index = 0;	// 最大的可读描述符索引
	char buf[MAX_BUF_LEN] = {0};

	while (true)
	{
		int ready_fd = poll(client_fd, max_read_fd_index + 1, -1);

		// 监听套接字消息
		if (client_fd[0].revents & POLLIN)
		{
			struct sockaddr_in client_addr;
			socklen_t client_addr_len = sizeof(client_addr);
			int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
			if (connect_fd < 0)
			{
				if (errno == EINTR)
				{
					continue;
				}
				else
				{
					printf("accept error.\n");
					fflush(stdout);
					return -1;
				}
			}
			else
			{
				printf("accept:fd=%d.\n", connect_fd);
				fflush(stdout);

				int i = 0;
				for (; i < OPEN_MAX; ++i)
				{
					if (client_fd[i].fd == -1)
					{
						client_fd[i].fd = connect_fd;
						client_fd[i].events = POLLIN;
						break;
					}
				}
				if (i == SELECT_FD_SET_SIZE)
				{
					printf("too many connection.\n");
					fflush(stdout);
					// 该咋办？
				}
				else
				{
					if (i > max_read_fd_index)
					{
						max_read_fd_index = i;
					}
				}
			}
			if (--ready_fd <= 0)
			{
				continue;
			}
		}

		// 可读套接字消息
		for (int i = 0; i <= max_read_fd_index; ++i)
		{
			if (client_fd[i].fd < 0)
			{
				continue;
			}
			if (client_fd[i].revents & (POLLIN | POLLERR))
			{
				memset(buf, 0, MAX_BUF_LEN);
				ssize_t len = read(client_fd[i].fd, buf, MAX_BUF_LEN);
				if (len > 0)
				{
					printf("recv:: %s\n", buf);

					memset(buf, 0, MAX_BUF_LEN);
					snprintf(buf, MAX_BUF_LEN, "hello client.");
					write(client_fd[i].fd, buf, strlen(buf));
				}
				else if (len < 0 && errno == EINTR)
				{
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
			if (--ready_fd <= 0)
			{
				continue;
			}
		}
	}

	return 0;
}
