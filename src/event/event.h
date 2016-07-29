
#define EVENT_SIZE					10240

#define MAX_EVENT_NUM				1024

#define EVENT_NONE					0
#define EVENT_READ_ABLE				1
#define EVENT_WRITE_ABLE			2

typedef struct EventLoop EventLoop;

typedef struct FileEvent FileEvent;

typedef struct TimeEvent TimeEvent;

typedef struct epoll_event EpollEvent;

typedef void (*FileProcessor)(EventLoop *eventLoop, int fd, int mask, Object *client);

typedef void (*TimeProcessor)(EventLoop *eventLoop, TimeEvent *timeEvent, Object *client);

typedef struct FileEvent {

	int fd;

	int mask;

	Object *client;

	FileProcessor readFileProcessor;
	
	FileProcessor writeFileProcessor;

} FileEvent;

typedef struct TimeEvent {
	
	u64 time;

	TimeProcessor timeProcessor;
	
} TimeEvent;

typedef struct FireEvent {

	int fd;

	int mask;	

} FireEvent;

typedef struct EpollState {

	int epollFd;

	EpollEvent epollEvents[EVENT_SIZE];

} EpollState;

typedef struct EventLoop {

	EpollState *epollState;

	TimeEvent *timeEvent;
	
	FileEvent fileEventList[EVENT_SIZE];

	FireEvent fireEventList[EVENT_SIZE];
	
} EventLoop;

EventLoop *createEventLoop();

void executeEventLoop(EventLoop *eventLoop);

void createFileEvent(EventLoop *eventLoop, int fd, int mask, FileProcessor processor, Object *client);

void createTimeEvent(EventLoop *eventLoop, TimeProcessor processor, u64 time, Object *client);

void deleteFileEvent(EventLoop *eventLoop, int fd, int mask);

