
typedef struct EventLoop EventLoop;

typedef struct String String;

typedef struct Client Client;

typedef void (*ReplyToClient)(Client *client, String *info);

typedef struct Client {

	int clientFd;

	int paramNum;

	String **params;

	String *replyInfo;

	ReplyToClient replyToClient;
	
} Client;

Client *createClient(int clientFd);

void processClientCommand(EventLoop *eventLoop, int fd, int mask, Object *client);
