#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <errno.h>

#include "../type/type.h"
#include "../event/event.h"
#include "../data/hashMap.h"
#include "../cache/cacheDB.h"
#include "../config/config.h"
#include "../const/const.h"
#include "network.h"

extern Server server;

int serverSocketFd;

int clientSocketFd;

int addressLength;

EpollEvent serverEvent;

EpollEvent clientEvent;

EpollEvent targetEvent[MAX_CONNECT];

int prepareServerSocket() {

	SocketInetAddress serverAddress, clientAddress;

	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd==-1) {
		printLog(LEVEL_ERROR, "Create Socket Error!");
		return -1;
	} else {
		printLog(LEVEL_INFO, "Create Socket Successfully^_^");	
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(server.serverPort);

	int status = bind(serverSocketFd, (SocketAddress *)&serverAddress, sizeof(SocketAddress));
	if (status==-1) {
		printLog(LEVEL_ERROR, "Bind Socket Error!");
		return -1;	
	} else {
		printLog(LEVEL_INFO, "Bind Socket Successfully^_^");	
	}

	status = listen(serverSocketFd, 20);
	if (status==-1) {
		printLog(LEVEL_ERROR, "Bind Socket Error:");
		return -1;	
	} else {
		printLog(LEVEL_INFO, "Start to Listen Successfully^_^");
	}
	return serverSocketFd;
}

int acceptClientConn(int serverFd, char *ip, int *port) {
	int clientFd = 0;
	SocketInetAddress address;
	u32 addressLength = sizeof(SocketInetAddress);
	while(true) {
		clientFd = accept(serverFd, (SocketAddress *)&address, &addressLength);
		if (clientFd==-1) {
			if (errno==EINTR) {
				continue;			
			} else {
				printLog(LEVEL_ERROR, "Accept Connection Error:");
				return INET_ERROR;
			}
		}
		break;
	}
	if (ip!=NULL) {
		strcpy(ip, inet_ntoa(address.sin_addr));	
	}
	if (port!=NULL) {
		*port = ntohs(address.sin_port);	
	}
	return clientFd;
}

void startDataService() {
	
	/*memset(&sendBuffer, 0, sizeof(sendBuffer));
	memset(&receiveBuffer, 0, sizeof(receiveBuffer));

	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd==-1) {
		printLog(LEVEL_ERROR, "Create Socket Error!");
		exit(0);
	} else {
		printLog(LEVEL_INFO, "Create Socket Successfully^_^");	
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(SERVER_PORT);

	int status = bind(serverSocketFd, (SocketAddress *)&serverAddress, sizeof(SocketAddress));
	if (status==-1) {
		printLog(LEVEL_ERROR, "Bind Socket Error!");
		exit(0);	
	} else {
		printLog(LEVEL_INFO, "Bind Socket Successfully^_^");	
	}

	status = listen(serverSocketFd, 20);
	if (status==-1) {
		printLog(LEVEL_ERROR, "Bind Socket Error!");
		exit(0);	
	} else {
		printLog(LEVEL_INFO, "Start to Listen Successfully^_^");
	}

	int epollFd = epoll_create(MAX_CONNECT);
	if (epollFd==-1) {
		printLog(LEVEL_ERROR, "Create Epoll Error");
		exit(0);	
	} else {
		printLog(LEVEL_INFO, "Create Epoll Successfully^_^");		
	}

	serverEvent.data.fd = serverSocketFd;
	serverEvent.events = EPOLLIN;

	status = epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocketFd, &serverEvent);  
  	if (status == -1) {  
      	printLog(LEVEL_ERROR, "Epoll_ctl Error");  
      	exit(0);	
    } else {
		printLog(LEVEL_INFO, "Epoll_ctl Successfully^_^");
	}

	while (true) {
		int eventNum = 0, i = 0;
		eventNum = epoll_wait(epollFd, targetEvent, MAX_CONNECT, -1);
		for (i = 0; i<eventNum; i++) {
	  		if ((targetEvent[i].events & EPOLLERR) ||
              	(targetEvent[i].events & EPOLLHUP) ||
              	(!(targetEvent[i].events & EPOLLIN))) 
	    	{
				printLog(LEVEL_ERROR, "Epoll Event error");
				close (targetEvent[i].data.fd);
				continue;
	    	} else if (serverSocketFd==targetEvent[i].data.fd) {


				printf("serverSocketFd:%d\n", serverSocketFd);
				printf("targetEvent[i].data.fd:%d\n", targetEvent[i].data.fd);


				addressLength = sizeof(SocketAddress);
				clientSocketFd = accept(serverSocketFd, (SocketAddress *)&clientAddress, &addressLength);
				if(clientSocketFd == -1) {
				  	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
				  	} else {
					  printLog(LEVEL_ERROR, "Accept Error");
				  	}
				} else {
					printLog(LEVEL_INFO, "Client Connection Accepted ^_^");
				}

				clientEvent.data.fd = clientSocketFd;
				clientEvent.events = EPOLLIN;
				status = epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocketFd, &clientEvent);
				if (status == -1) {
				  	printLog(LEVEL_ERROR, "Epoll_ctl Error");
				  	exit(0);
				}
            } else {
				int done = 0;
				while (true) {
				  long count;
				  char buf[512];

				  count = read(targetEvent[i].data.fd, receiveBuffer, sizeof(receiveBuffer));
				  if(count == -1) {
					  if (errno != EAGAIN) {
						  printLog(LEVEL_ERROR, "read");
						  done = 1;
					  }
					  break;
				  } else if(count == 0) {
					  done = 1;
					  break;
				  }
				  u64 writeBytes = write(1, sendBuffer, count);
				  if (writeBytes == -1) {
					  printLog(LEVEL_ERROR, "Write Error");
					  exit(0);
				  }

				  printLog(LEVEL_INFO, receiveBuffer);
				}

				if(done) {
				  printLog(LEVEL_INFO, "Closed Connection on Descriptor");
				  close (targetEvent[i].data.fd);
				}
		   }
        }
	}*/
}

