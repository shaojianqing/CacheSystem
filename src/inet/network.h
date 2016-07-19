#define SERVER_PORT 			8888
#define MAX_CONNECT				1024

typedef struct sockaddr_in SocketInetAddress;

typedef struct sockaddr SocketAddress;

typedef struct epoll_event EpollEvent;

void startService();
