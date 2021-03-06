#include "../common/header.h"

void server_func(int fd)
{
	char buf[MAX_BUF_LEN] = {0};

	while (true)
	{
		memset(buf, 0, MAX_BUF_LEN);
		ssize_t len = read(fd, buf, MAX_BUF_LEN);
		if (len > 0)
		{
			printf("recv:: %s\n", buf);

			memset(buf, 0, MAX_BUF_LEN);
			snprintf(buf, MAX_BUF_LEN, "hello client.");
			write(fd, buf, strlen(buf));
			printf("send:: %s\n", buf);
			fflush(stdout);
			continue;
		}
		else if ((len == -1 && errno == EINTR) || errno == EAGAIN)
		{
			// 慢系统调用导致信号中断没有读到数据 或 非阻塞没有数据则返回EAGAIN
			printf("recv error:: continue\n");
			fflush(stdout);
			continue;
		}
		printf("connection closed:fd=%d\n", fd);
		fflush(stdout);
		break;
	}
	close(fd);
}

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

	while (true)
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
		printf("accept:fd=%d.\n", connect_fd);
		fflush(stdout);

		std::thread th(server_func, connect_fd);
		th.detach();
	}

	close(listen_fd);
	return 0;
}
