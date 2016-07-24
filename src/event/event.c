#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/epoll.h>

#include "../const/const.h"
#include "../type/type.h"
#include "event.h"

static EpollState* createEpollState();

static bool registeEpollEvent(EventLoop *loop, int fd, int mask);

static void processTimeEvent(EventLoop *eventLoop);

static void processFileEvent(EventLoop *eventLoop);

static void processAllEvents(EventLoop *eventLoop);

EventLoop* createEventLoop() {
	EventLoop *eventLoop = (EventLoop *)malloc(sizeof(EventLoop));
	if (eventLoop!=NULL) {
		EpollState *epollState = createEpollState();
		if (epollState!=NULL) {
			eventLoop->epollState = epollState;
			return eventLoop;
		}
	}
	return NULL;	
}

static EpollState* createEpollState() {
	EpollState *state = (EpollState *)malloc(sizeof(EpollState));
	if (state!=NULL) {
		state->epollFd = epoll_create(1024);
		if (state->epollFd>=0) {
			return state;
		}
	}
	return NULL;
}

void executeEventLoop(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		while (true) {
			processAllEvents(eventLoop);
		}
	}
}

void createFileEvent(EventLoop *eventLoop, int fd, int mask, FileProcessor processor, Object *client) {
	if (eventLoop!=NULL && fd<EVENT_SIZE) {
		FileEvent *fileEvent = &(eventLoop->fileEventList[fd]);
		if (registeEpollEvent(eventLoop, fd, mask)) {
			fileEvent->mask |= mask;
			fileEvent->client = client;
			if (mask & EVENT_READ_ABLE) {
				fileEvent->readFileProcessor = processor;
			}
			if (mask & EVENT_WRITE_ABLE) {
				fileEvent->writeFileProcessor = processor;
			}
		}		
	}
}

void createTimeEvent(EventLoop *eventLoop, TimeProcessor processor, u64 time, Object *client) {
	if (eventLoop!=NULL) {
		TimeEvent *timeEvent = (TimeEvent *)malloc(sizeof(TimeEvent));
		if (timeEvent!=NULL) {
			timeEvent->time = time;
			timeEvent->timeProcessor = processor;
			eventLoop->timeEvent = timeEvent;
		}
	}
}

static bool registeEpollEvent(EventLoop *eventLoop, int fd, int mask) {
	if (eventLoop!=NULL) {
		EpollState *epollState = eventLoop->epollState;
		EpollEvent epollEvent;

		FileEvent fileEvent = eventLoop->fileEventList[fd];
		int operator = (fileEvent.mask==EVENT_NONE?EPOLL_CTL_ADD:EPOLL_CTL_MOD);
		epollEvent.events = 0;
		epollEvent.data.fd = fd;

		mask |= fileEvent.mask;
		if (mask & EVENT_READ_ABLE) {
			epollEvent.events = EPOLLIN;
		}
		if (mask & EVENT_WRITE_ABLE) {
			epollEvent.events = EPOLLOUT;
		}
		if (epoll_ctl(epollState->epollFd, operator, fd, &epollEvent)==-1) {
			return false;		
		} else {
			return true;		
		}
	}
	return false;
}

static int processEpollEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		EpollState *epollState = eventLoop->epollState;
		TimeEvent *timeEvent = eventLoop->timeEvent;
		int timeSlice = (timeEvent!=NULL?timeEvent->time:-1);
		int eventNum = epoll_wait(epollState->epollFd, epollState->epollEvents, MAX_EVENT_NUM, timeSlice);
		if (eventNum>0) {
			int i = 0, mask = 0;
			for (i=0;i<eventNum;++i) {
				EpollEvent event = epollState->epollEvents[i];
				if (event.events & EPOLLIN) {
					mask |= EVENT_READ_ABLE;
				}
				if (event.events & EPOLLOUT) {
					mask |= EVENT_WRITE_ABLE;				
				}
				eventLoop->fireEventList[i].fd = event.data.fd;
				eventLoop->fireEventList[i].mask = mask;
			}	
		}
		return eventNum;
	}
	return 0;
}

static void processAllEvents(EventLoop *eventLoop) {
	processTimeEvent(eventLoop);
	processFileEvent(eventLoop);
}

static void processTimeEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		TimeEvent *timeEvent = eventLoop->timeEvent;
		if (timeEvent!=NULL && timeEvent->timeProcessor!=NULL) {
			timeEvent->timeProcessor(eventLoop, timeEvent, NULL);
		}
	}
}

static void processFileEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		int eventNum = processEpollEvent(eventLoop);
		if (eventNum>0) {
			int i = 0, mask = 0;
			for (i=0;i<eventNum;++i) {
				FireEvent fireEvent = eventLoop->fireEventList[i];
				FileEvent fileEvent = eventLoop->fileEventList[fireEvent.fd];
				if (fileEvent.mask & fireEvent.mask & EVENT_READ_ABLE) {
					fileEvent.readFileProcessor(eventLoop, fireEvent.fd, fireEvent.mask, fileEvent.client);
				}

				if (fileEvent.mask & fireEvent.mask & EVENT_WRITE_ABLE) {
					fileEvent.writeFileProcessor(eventLoop, fireEvent.fd, fireEvent.mask, fileEvent.client);
				} 
			}
		}
	}
}
