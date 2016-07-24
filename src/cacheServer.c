#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "type/type.h"
#include "event/event.h"
#include "config/config.h"
#include "data/dataType.h"
#include "data/hashMap.h"
#include "data/arrayList.h"
#include "cacheServer.h"

extern Server server;

int main(int argc, char **argv) {

	initServerConfig();
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

	initServerFacility();
	//startDataService();
	executeEventLoop(server.eventLoop);
	
	//startDataService();

	printLog(LEVEL_INFO, "Cache Service is now running^_^");
}
