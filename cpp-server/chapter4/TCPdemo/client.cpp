/**
 * TCP客户端通信基本流程
 * zhangyl 2018.12.13
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define SERVER_ADDRESS "192.168.1.253"
#define SERVER_PORT     20103
static const char * SEND_DATA = "GET /sys/getTime HTTP/1.1\r\n"
                        "User-Agent: PostmanRuntime/7.29.0\r\n"
                        "Accept: */*\r\n"
                        "Postman-Token: bfd1d096-58dc-4e0b-854a-55deeb081246\r\n"
                        "Host: 192.168.1.253:20103\r\n"
                        "Accept-Encoding: gzip, deflate, br\r\n"
                        "Connection: keep-alive\r\n\r\n";

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        return -1;
    }
    //1.1 增加客户端绑定端口
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(0);
    if (bind(clientfd,(struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1){
        std::cout << "bind socket error." << std::endl;
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
		close(clientfd);
        return -1;
    }

	//3. 向服务器发送数据
	int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
	if (ret != strlen(SEND_DATA))
	{
		std::cout << "send data error." << std::endl;
		close(clientfd);
		return -1;
	}
	
	std::cout << "send data successfully, data: "<< std::endl << SEND_DATA << std::endl;
	
	//4. 从服务器收取数据
	char recvBuf[1024] = {0};
	ret = recv(clientfd, recvBuf, 1024, 0);
	if (ret > 0) 
	{
		std::cout << "recv data successfully, data: " << recvBuf << std::endl;
	} 
	else 
	{
		std::cout << "recv data error, data: " << recvBuf << std::endl;
	}
	
	//5. 关闭socket
	close(clientfd);

    return 0;
}