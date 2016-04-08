.PHONY:all
all:tcp_server tcp_client

tcp_server:tcp_server.c
	gcc -o tcp_server tcp_server.c -lpthread  -D_FUNC3_
tcp_client:tcp_client.c
	gcc -o tcp_client tcp_client.c 

.PHONY:clean
clean:
	rm -rf tcp_server tcp_client
