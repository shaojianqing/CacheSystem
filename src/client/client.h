
typedef struct Client Client;

typedef void (*ReplyToClient)(Client *client, String *info);

struct Client {

	int clientFd;

	int paramNum;

	String **params;

	String *replyInfo;

	ReplyToClient replyToClient;
};

Client *createClient(int clientFd);

void processClientCommand(EventLoop *eventLoop, int fd, int mask, Object *client);
