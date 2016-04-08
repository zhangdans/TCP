#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#define _PORT_ 8088
int main(int argc,char*argv[])
{
	int client_sock=socket(AF_INET/*ipv4*/,SOCK_STREAM,0);
	if(client_sock<0)
	{
		perror("socket");
		exit(1);
	}

	char*ip=argv[1];
	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	remote.sin_port=htons(_PORT_);
	inet_aton(ip,&remote.sin_addr);

	int ret=connect(client_sock,(struct sockaddr*)&remote,sizeof(remote));
	if(ret<0)
	{
		perror("connect");
		exit(1);
	}
	//connect success
	char buf[1024];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		gets(buf);

		ssize_t len=write(client_sock,buf,strlen(buf));
		if(len<0)
		{
			perror("write");
			break;
		}
	}
	close(client_sock);
	
	return 0;
}
