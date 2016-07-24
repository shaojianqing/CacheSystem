#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "../type/type.h"
#include "../event/event.h"
#include "../client/client.h"
#include "config.h"

Server server;

void timingHandler(EventLoop *eventLoop, TimeEvent *timeEvent, Object *client);

void acceptConnHandler(EventLoop *eventLoop, int fd, int mask, Object *client);

void initServerConfig() {
	server.serverPort = 8888;
	server.logFile = NULL;//"/home/shaojianqing/CacheSystem/cache.log";
	server.timeSlice = TIME_SLICE;
	server.times = 0;
}

void initServerFacility() {
	server.eventLoop = createEventLoop();
	server.serverFd = prepareServerSocket();
	createTimeEvent(server.eventLoop, timingHandler, TIME_SLICE, NULL);
	createFileEvent(server.eventLoop, server.serverFd, EVENT_READ_ABLE, acceptConnHandler, NULL);
}

void loadServerConfig(const char *filename) {
	if (filename!=NULL) {
	
	}
}

void printLog(u8 level, const char *message) {

	char *levelStr = NULL;
	char *errorStr = "";
	if (level==LEVEL_INFO) {
		levelStr = "[INFO]";
	} else if (level==LEVEL_WARN) {
		levelStr = "[WARN]";
	} else if (level==LEVEL_ERROR) {
		levelStr = "[ERROR]";
		errorStr = strerror(errno);
	}

	long now = time(NULL);
	char dateBuffer[32];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
		
	FILE *logFile = NULL;
	if (server.logFile!=NULL) {
		logFile = fopen(server.logFile,"a");
	}
	if (logFile==NULL) {
		logFile = stdout;	
	}

	fprintf(logFile, "%s %s %s %s\n", dateBuffer, levelStr, message, errorStr);
	fflush(logFile);
	if (server.logFile!=NULL) {
		fclose(logFile);	
	}
}

void printHelpInfo() {
	printf("		Welcome to using CacheSystem ^_^        \n");
	printf("The CacheSystem is one key-value mapping database, it can be used\n");
	printf("in the server side system and cluster, serving as the cache database.\n");
	printf("The CacheSystem has very high performance, as it takes the epoll as \n");
	printf("the IO event notification facility that can support million connections.\n");
	printf("The CacheSystem can be currently deployed only on the linux platform.\n");
}

void acceptConnHandler(EventLoop *eventLoop, int fd, int mask, Object *client) {
	char msg[100];
	char ip[32];
	int port;
	int clientFd = acceptClientConn(fd, ip, &port);
	if (clientFd!=-1) {
		sprintf(msg, "Connecting client address:%s, port:%d\n", ip, port);
		printLog(LEVEL_INFO, msg);
		Client *client = (Client *)malloc(sizeof(Client));
		if (client!=NULL) {
			client->clientFd = clientFd;
			createFileEvent(eventLoop, clientFd, EVENT_READ_ABLE, processClientCommand, client);
		}
	}
}

void timingHandler(EventLoop *eventLoop, TimeEvent *timeEvent, Object *client) {
	server.systemTime = time(NULL);
	server.times++;
	if ((server.times%50)==0) {
		printLog(LEVEL_INFO, "Timing Event Output");
	}	
}
