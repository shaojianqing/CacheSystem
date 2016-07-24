#define SERVER_PORT 			8888
#define MAX_CONNECT				1024

#define INET_ERROR				-1
#define INET_SUCCESS			1

typedef struct sockaddr_in SocketInetAddress;

typedef struct sockaddr SocketAddress;

typedef struct epoll_event EpollEvent;

int prepareServerSocket();

int acceptClientConn(int serverFd, char *ip, int *port);

