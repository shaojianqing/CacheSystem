#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "../type/type.h"
#include "../event/event.h"
#include "../data/stringType.h"
#include "../data/dataType.h"
#include "../data/hashMap.h"
#include "../cache/cacheDB.h"
#include "../client/client.h"
#include "config.h"

Server server;

ReplyInfo replyInfo;

void timingHandler(EventLoop *eventLoop, TimeEvent *timeEvent, Object *client);

void acceptConnHandler(EventLoop *eventLoop, int fd, int mask, Object *client);

void loadServerConfig(const char *filename);

void initDefaultConfig() {
	server.serverPort = 6379;
	server.logFile = NULL;//"/home/shaojianqing/CacheSystem/cache.log";
	server.timeSlice = TIME_SLICE;
	server.times = 0;
}

void initRepyInfomation() {
	replyInfo.ok = createString("+OK\r\n");
	replyInfo.err = createString("-ERR\r\n");
	replyInfo.noKeyErr = createString("-ERR no such key\r\n");
	replyInfo.synTaxErr = createString("-ERR syntax error\r\n");
}

void initServerConfig(int argc, char **argv) {
	initDefaultConfig();
	initRepyInfomation();
	if (argc==2) {
		char *content = argv[1];
		if (strcmp(content, "-h")==0) {
			printHelpInfo();
			exit(0);		
		} else {
			char *configFilename = content;
			loadServerConfig(configFilename);		
		}		
	} else if (argc==1) {
		char *info = "No config file is specified, the server will load the default config!";
		printLog(LEVEL_WARN, info);
	} else {
		char *info = "The input command is invalid!";
		printLog(LEVEL_ERROR, info);
	}
}

void initServerFacility() {
	server.cacheDB = createCacheDB();
	server.eventLoop = createEventLoop();
	server.serverFd = prepareServerSocket();
	createTimeEvent(server.eventLoop, timingHandler, TIME_SLICE, NULL);
	createFileEvent(server.eventLoop, server.serverFd, EVENT_READ_ABLE, acceptConnHandler, NULL);

	printLog(LEVEL_INFO, "Cache Service is now running^_^");
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
		Client *client = createClient(clientFd);
		if (client!=NULL) {
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
