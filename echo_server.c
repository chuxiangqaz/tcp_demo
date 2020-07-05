#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	// 服务端socket（相当于门卫）
	int serv_socket;
	// 客户端socket（客户端与服务端的通讯的socket）
	int clnt_socket;
	
	// 服务端地址信息
	struct sockaddr_in serv_addr;
	// 客户端地址信息
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	int str_len, i;
	
	char message[] = "Hello Word!";
	
	if (argc != 2) 
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	
	// 创建一个socket 链接
	serv_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_socket == -1) 
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if (bind(serv_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind error");
	}
	
	if (listen(serv_socket, 5) == -1)
	{
		error_handling("listen() error");
	}
	
	clnt_addr_size = sizeof(clnt_addr);
	for (i = 0; i < 5; i++)
	{
		clnt_socket = accept(serv_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_socket == -1)
		{
			error_handling("accept() error");
		} else {
			printf("Connected client %d \n", i);
		}

		while ((str_len=read(clnt_socket, message, BUF_SIZE)) != 0)
		{
			printf("服务器收到的长度 %d，内容为：%s 完了",str_len, message);
			write(clnt_socket, message, str_len);
		}

		close(clnt_socket);
	}
	
	close(serv_socket);	
	return 0;
	
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputs("\n",stderr);
	exit(1);
}
