#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "type/type.h"
#include "event/event.h"
#include "config/config.h"
#include "cacheServer.h"

extern Server server;

int main(int argc, char **argv) {

	/*String *name = createString("shaojianqing");

	String *subName = name->subString(name, 2, 8);

	String *catName = createString("TestTestTestTestTestTestTest");

	String *newName = name->catString(name, catName);	

	printf("The String is:%s the length is:%d\n", newName->value, newName->length);

	printLog(LEVEL_INFO, newName->value);*/

	initServerConfig(argc, argv);
	initServerFacility();
	executeEventLoop(server.eventLoop);
}
