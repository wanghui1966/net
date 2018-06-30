#include "header.h"

void client_func(int fd)
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	getsockname(fd, (struct sockaddr*)&client_addr, &client_addr_len);
	char *client_ip = inet_ntoa(client_addr.sin_addr);
	short client_port = client_addr.sin_port;

	char send_buf[MAX_BUF_LEN] = {0};
	char recv_buf[MAX_BUF_LEN] = {0};

	int index = 0;
	while (true)
	{
		sleep(3);
		fflush(stdout);

		memset(send_buf, 0, MAX_BUF_LEN);
		snprintf(send_buf, MAX_BUF_LEN, "hello server(ip=%s, port=%d, pid=%d, index=%d).", client_ip, client_port, getpid(), index++);
		write(fd, send_buf, strlen(send_buf));
		printf("send:: %s\n", send_buf);

		memset(recv_buf, 0, MAX_BUF_LEN);
		ssize_t len = read(fd, recv_buf, MAX_BUF_LEN);
		if (len > 0)
		{
			printf("recv:: %s\n", recv_buf);
			continue;
		}
		else if (len < 0 && errno == EINTR)
		{
			printf("recv error:: continue\n");
			continue;
		}
		printf("recv error:: quit\n");
		break;
	}
}

int main()
{
	// 创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		printf("scoket error.\n");
		return 1;
	}

	// 服务器套接字
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// 连接服务器
	if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("connect error.\n");
		return 1;
	}
	client_func(fd);

	return 0;
}
