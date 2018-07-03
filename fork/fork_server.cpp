#include "../common/header.h"

void sig_chld(int signo)
{
	while (true)
	{
		int stat = 0;
		pid_t pid = waitpid(-1, &stat, WNOHANG);
		if (pid > 0)
		{
			printf("fork_client(pid=%d) terminated.\n", pid);
			fflush(stdout);
			continue;
		}
		break;
	}
}

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
		else if (len < 0 && errno == EINTR)
		{
			printf("recv error:: continue\n");
			fflush(stdout);
			continue;
		}
		printf("recv error:: quit\n");
		fflush(stdout);
		break;
	}
}

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

	signal(SIGCHLD, sig_chld);
	
	while (true)
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
				return -1;
			}
		}
		printf("accept:fd=%d.\n", connect_fd);
		fflush(stdout);

		pid_t child_pid = fork();
		if (child_pid == 0)
		{
			close(listen_fd);
			server_func(connect_fd);
			return 0;
		}
		else if (child_pid > 0)
		{
			close(connect_fd);
		}
		else
		{
			printf("fork error.\n");
			fflush(stdout);
			close(connect_fd);
		}
	}

	return 0;
}
