#include "header.h"

void client_func(int fd)
{
	char send_buf[MAX_BUF_LEN] = {0};
	char recv_buf[MAX_BUF_LEN] = {0};

	int index = 0;
	while (true)
	{
		sleep(3);

		memset(send_buf, 0, MAX_BUF_LEN);
		snprintf(send_buf, MAX_BUF_LEN, "hello server(pid=%d, index=%d).", getpid(), index++);
		write(fd, send_buf, strlen(send_buf));
		printf("send:: %s\n", send_buf);

		memset(recv_buf, 0, MAX_BUF_LEN);
		ssize_t len = read(fd, recv_buf, MAX_BUF_LEN);
		if (len > 0)
		{
			printf("recv:: %s\n", recv_buf);
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
	int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd < 0)
	{
		printf("scoket error.\n");
		return 1;
	}

	// 服务器套接字
	struct sockaddr_un server_addr;
	server_addr.sun_family = AF_LOCAL;
	strcpy(server_addr.sun_path, UNIX_FILE);

	// 连接服务器
	if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("connect error.\n");
		return 1;
	}
	client_func(fd);

	close(fd);
	return 0;
}
