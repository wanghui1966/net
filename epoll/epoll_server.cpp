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

	int epoll_fd = epoll_create(MAX_FD_NUM);
	if (epoll_fd < 0)
	{
		printf("epoll_create error.\n");
		close(listen_fd);
		return -1;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1)
	{
		printf("epoll_ctl listen_fd error.\n");
		close(listen_fd);
		close(epoll_fd);
		return -1;
	}
	struct epoll_event events[MAX_FD_NUM];
	char buf[MAX_BUF_LEN] = {0};

	while (true)
	{
		int ready_fd = epoll_wait(epoll_fd, events, MAX_FD_NUM, -1);
		if (ready_fd <= 0)
		{
			printf("epoll_wait error: continue.\n");
			fflush(stdout);
			continue;
		}

		for (int i = 0; i < ready_fd; ++i)
		{
			// 监听套接字消息
			if ((events[i].data.fd == listen_fd) && (events[i].events & (EPOLLIN | EPOLLERR | EPOLLHUP)))
			{
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);
				int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
				if (connect_fd < 0)
				{
					printf("accept error: continue.\n");
				}
				else
				{
					printf("accept:fd=%d.\n", connect_fd);

					ev.events = EPOLLIN;
					ev.data.fd = connect_fd;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_fd, &ev) == -1)
					{
						printf("epoll_ctl connect_fd(%d) error: continue.\n", connect_fd);
					}
				}
				fflush(stdout);
				continue;
			}

			if (events[i].events & (EPOLLIN | EPOLLERR | EPOLLHUP))	
			{
				// 可读事件或错误
				memset(buf, 0, MAX_BUF_LEN);
				ssize_t len = read(events[i].data.fd, buf, MAX_BUF_LEN);
				if (len > 0)
				{
					printf("recv:: %s\n", buf);

					memset(buf, 0, MAX_BUF_LEN);
					snprintf(buf, MAX_BUF_LEN, "hello client.");
					write(events[i].data.fd, buf, strlen(buf));
				}
				else if ((len == -1 && errno == EINTR) || errno == EAGAIN)
				{
					// 慢系统调用导致信号中断没有读到数据 或 非阻塞没有数据则返回EAGAIN
					printf("recv error:: continue\n");
				}
				else
				{
					ev.events = EPOLLIN;
					ev.data.fd = events[i].data.fd;
					int err = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
					if (err != -1)
					{
						close(events[i].data.fd);
					}
					printf("epoll_ctl fd(%d): err=%d.\n", events[i].data.fd, err);
				}
				fflush(stdout);
			}
			else if (events[i].events & (EPOLLOUT))
			{
				// 可写事件
			}
			else
			{
				// 关闭连接
			}
		}
	}

	close(listen_fd);
	close(epoll_fd);
	return 0;
}
