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

	initServerConfig(argc, argv);
	initServerFacility();
	executeEventLoop(server.eventLoop);
}
