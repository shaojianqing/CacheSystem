
typedef struct Client {

	int clientFd;

	
	
} Client;

void processClientCommand(EventLoop *eventLoop, int fd, int mask, Object *client);
