#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "../type/type.h"
#include "../event/event.h"
#include "../config/config.h"
#include "client.h"

char sendBuffer[1024];

char receiveBuffer[1024];

void processClientCommand(EventLoop *eventLoop, int fd, int mask, Object *client) {
	if (eventLoop!=NULL) {
		int done = 0;
		long count;
		char buf[512];

		count = read(fd, receiveBuffer, sizeof(receiveBuffer));
		if(count == -1) {
			if (errno != EAGAIN) {
				printLog(LEVEL_ERROR, "Read Error!");
				done = 1;
			}
		} else if(count == 0) {
			done = 1;
		}

		char *info = "This is Server Side^&^\n";
		strcpy(sendBuffer, info);

		u64 writeBytes = write(fd, sendBuffer, strlen(info));
		if (writeBytes == -1) {
			printLog(LEVEL_ERROR, "Write Error!");
		}

		printLog(LEVEL_INFO, receiveBuffer);
	}
}
