#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "../type/type.h"
#include "../event/event.h"
#include "../config/config.h"
#include "../data/stringType.h"
#include "client.h"

extern Server server;

extern ReplyInfo replyInfo;

static void replyToClient(Client *client, String *info);

static void sendReplyToClient(EventLoop *eventLoop, int fd, int mask, Object *object);

Client *createClient(int clientFd) {
	if (clientFd>0 && clientFd<EVENT_SIZE) {
		Client *client  = malloc(sizeof(Client));
		if (client!=NULL) {
			client->clientFd = clientFd;
			client->replyToClient = replyToClient;
			return client;
		}
	}
	return NULL;
}

void resolveCommand(Client *client, String *command) {
	if (client!=NULL && command!=NULL && command->length>0) {
		char *limit = command->value+command->length;
		char *query = command->value;
		char *param = strchr(query, '\n');
		if (param!=NULL) {
			*param = '\0';
			if (*(param-1)=='\r') {
				*(param-1) = '\0';
			}		
		}
		if (query[0]=='*') {
			client->paramNum = atoi(query+1);
			client->params = malloc(sizeof(String *)*client->paramNum);
			query = param+1;
			int bitNum = 0;
			int i=0;
			while (query<limit) {
				param = strchr(query, '\n');
				if (param!=NULL) {
					*param = '\0';
					if (*(param-1)=='\r') {
						*(param-1) = '\0';
					}		
				}
				if (query[0]=='$') {
					bitNum = atoi(query+1);					
				} else {
					if (bitNum>0) {
						client->params[i] = createStringWithRange(query, bitNum);
						++i;				
					}
				}
				query=param+1;					
			}
		}
	}
}

void processClientCommand(EventLoop *eventLoop, int fd, int mask, Object *object) {
	if (eventLoop!=NULL && object!=NULL) {
		Client *client = (Client *)object;
		int done = 0;
		long count;
		char buf[512];

		char receiveBuffer[1024];
		memset(receiveBuffer, 0, 1024);

		count = read(fd, receiveBuffer, sizeof(receiveBuffer));
		if(count == -1) {
			if (errno != EAGAIN) {
				printLog(LEVEL_ERROR, "Read Error!");
				done = 1;
			}
		} else if(count == 0) {
			done = 1;
		}

		String *command = createString(receiveBuffer);
		resolveCommand(client, command);
		executeCommand(client);
	}
}

static void replyToClient(Client *client, String *info) {
	if (client!=NULL && info!=NULL) {
		client->replyInfo = info;
		createFileEvent(server.eventLoop, client->clientFd, EVENT_WRITE_ABLE, sendReplyToClient, client);
	}
}

static void sendReplyToClient(EventLoop *eventLoop, int fd, int mask, Object *object) {
	if (eventLoop!=NULL && object!=NULL) {
		Client *client = (Client *)object;
		String *reply = client->replyInfo;
		u64 writeBytes = write(fd, reply->value, reply->length);
		if (writeBytes == -1) {
			printLog(LEVEL_ERROR, "Write Error!");
		} else {
			printLog(LEVEL_INFO, "Success Send Reply to Client^+^");
		}
		deleteFileEvent(eventLoop, fd, mask);
	}
}
