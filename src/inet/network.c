#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "../const/const.h"
#include "../type/type.h"
#include "network.h"

char sendBuffer[1024];

char receiveBuffer[1024];

int serverSocketFd;

int clientSocketFd;

int addressLength;

SocketInetAddress serverAddress;

SocketInetAddress clientAddress;

EpollEvent serverEvent;

EpollEvent clientEvent;

EpollEvent targetEvent[MAX_CONNECT];

int makeSocketNoBlock(int socketFd);

void startService() {
	
	memset(&sendBuffer, 0, sizeof(sendBuffer));
	memset(&receiveBuffer, 0, sizeof(receiveBuffer));

	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd==-1) {
		perror("Create Socket Error!");
		exit(0);
	} else {
		printf("Create Socket Successfully^_^\n");	
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(SERVER_PORT);

	int status = bind(serverSocketFd, (SocketAddress *)&serverAddress, sizeof(SocketAddress));
	if (status==-1) {
		perror("Bind Socket Error!");
		exit(0);	
	} else {
		printf("Bind Socket Successfully^_^\n");	
	}

	status = makeSocketNoBlock(serverSocketFd);
	if (status==-1) {
		perror("Make Socket No Block Error!");
		exit(0);
	}

	status = listen(serverSocketFd, 20);
	if (status==-1) {
		perror("Bind Socket Error!");
		exit(0);	
	} else {
		printf("Start to Listen Successfully^_^\n");
	}

	int epollFd = epoll_create(MAX_CONNECT);
	if (epollFd==-1) {
		perror("Create Epoll Error");
		exit(0);	
	} else {
		printf("Create Epoll Successfully^_^\n");		
	}

	serverEvent.data.fd = serverSocketFd;
	serverEvent.events = EPOLLIN | EPOLLET;

	status = epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocketFd, &serverEvent);  
  	if (status == -1) {  
      	perror ("Epoll_ctl Error");  
      	exit(0);	
    } else {
		printf("Epoll_ctl Successfully^_^\n");
	}

	while (true) {
		int eventNum = 0, i = 0;
		eventNum = epoll_wait(epollFd, targetEvent, MAX_CONNECT, -1);
		for (i = 0; i<eventNum; i++) {
	  		if ((targetEvent[i].events & EPOLLERR) ||
              	(targetEvent[i].events & EPOLLHUP) ||
              	(!(targetEvent[i].events & EPOLLIN))) 
	    	{
				  perror("epoll error");
				  close (targetEvent[i].data.fd);
				  continue;
	    	} else if (serverSocketFd==targetEvent[i].data.fd) {

              while (true) {
				  	addressLength = sizeof(SocketAddress);
				  	clientSocketFd = accept(serverSocketFd, (SocketAddress *)&clientAddress, &addressLength);
                  	if(clientSocketFd == -1) {
                      	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                          break;
                      	} else {
                          perror ("accept");
                          break;
                      	}
                    } else {
						printf("Client Connection Accepted ^_^\n");
					}
                  	status = makeSocketNoBlock(clientSocketFd);
                  	if (status == -1) {
						exit(0);					
					}

		            clientEvent.data.fd = clientSocketFd;
		            clientEvent.events = EPOLLIN|EPOLLET;
                    status = epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocketFd, &clientEvent);
                  	if (status == -1) {
                      	perror ("epoll_ctl");
                      	exit(0);
                    }
                }
              continue;
            } else {
              int done = 0;
              while (true) {
                  long count;
                  char buf[512];

                  count = read (targetEvent[i].data.fd, receiveBuffer, sizeof(receiveBuffer));
                  if(count == -1) {
                      if (errno != EAGAIN) {
                          perror ("read");
                          done = 1;
                      }
                      break;
                  } else if(count == 0) {
                      done = 1;
                      break;
                  }
                  long writeBytes = write(1, receiveBuffer, count);
                  if (writeBytes == -1) {
                      perror ("write");
                      exit(0);
                  }
              }

              if(done) {
                  printf ("Closed connection on descriptor %d\n", targetEvent[i].data.fd);
                  close (targetEvent[i].data.fd);
              }
		   }
        }
	}	
}

int makeSocketNoBlock(int socketFd) {
	int flags, s;

	flags = fcntl(socketFd, F_GETFL, 0);
	if (flags == -1) {
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (socketFd, F_SETFL, flags);
	if (s == -1) {
		perror ("fcntl");
		return -1;
	}
	return 0;
}


