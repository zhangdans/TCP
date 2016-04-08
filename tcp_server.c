#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/wait.h>
//acquire listen socket
int listensocket(const char*ip,unsigned short port)
{
	int listen_sock=socket(AF_INET/*ipv4*/,SOCK_STREAM,0);
	if(listen_sock<0)
	{
		perror("socket");
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	inet_aton(ip,&local.sin_addr);

	int ret=bind(listen_sock,(struct sockaddr*)&local,sizeof(local));
	if(ret<0)
	{
		perror("bind");
		exit(1);
	}
	//bind success
	
	ret=listen(listen_sock,5);
	if(ret<0)
	{
		perror("listen");
		exit(1);
	}
	return listen_sock;
}
void handle(int sig)
{
	while(waitpid(-1,NULL,WNOHANG)==-1)
	{
		;
	}
}
void* thread_run(void*arg)
{
		int data_sock=(int)arg;
		char buf[1024];
		while(1)
		{
			memset(buf,'\0',sizeof(buf));
			ssize_t len=read(data_sock,buf,sizeof(buf)-1);
			if(len<0)
			{
				perror("read");
				break;
			}
			if(len==0)
			{
				printf("client release\n");
				break;
			}
			else
			{
				printf("client say->:%s\n",buf);
			}
		}
	close(data_sock);
	return NULL;
}
	
int main(int argc,char*argv[])
{	
	char*ip=argv[1];
	unsigned short port=atoi(argv[2]);
	int listen_sock=listensocket(ip,port);
	
	struct sockaddr_in client_info;
	int client_info_len=0;

	while(1)
	{
		signal(SIGCHLD,handle);
		int data_sock=accept(listen_sock,(struct sockaddr*)&client_info,&client_info_len);
		if(data_sock<0)
		{
			perror("accept");
			exit(1);
		}
		printf("get a new link\n");
#ifdef _FUNC1_
		char buf[1024];
		while(1){
			memset(buf,'\0',sizeof(buf));
			ssize_t len=read(data_sock,buf,sizeof(buf)-1);
			if(len<0){
				perror("read");
				break;
			}
			if(len==0){
				printf("client release\n");
				break;
			}
			else
			{
				printf("client say->:%s\n",buf);
			}
		}
#endif  //_FUNC1_

#ifdef _FUNC2_
		pid_t id=fork();
		if(id<0){
			perror("fork");
			exit(1);
		}
		else if(id==0){
			close(listen_sock);
			char buf[1024];
			while(1){
				memset(buf,'\0',sizeof(buf));
				ssize_t len=read(data_sock,buf,sizeof(buf)-1);
				if(len<0){
					perror("read");
					break;
				}	
				if(len==0){
					printf("client release\n");
					break;
				}
				else{
					printf("client say->:%s\n",buf);
				}
			}
			close(data_sock);
			exit(1);
		}
		close(data_sock);
#endif  //_FUNC2__
#ifdef _FUNC3_
		pthread_t tid;
		pthread_create(&tid,NULL,thread_run,(void*)data_sock);
		pthread_detach(tid);
#endif //_FUNC3_
	}
	close(listen_sock);
	return 0;
}


