#define LEVEL_INFO			1
#define LEVEL_WARN			2
#define LEVEL_ERROR			3

#define TIME_SLICE			100

typedef struct Server {

	u32 serverPort;

	char *logFile;

	EventLoop *eventLoop;

	int serverFd;

	u64 times;

	u64 systemTime;

	u64 timeSlice;

	CacheDB *cacheDB;

} Server;

typedef struct ReplyInfo {

	String *ok;

	String *err;

	String *noKeyErr;

	String *synTaxErr;	

} ReplyInfo;

void initServerConfig(int argc, char **argv);

void initServerFacility();

void loadServerConfig(const char *filename);

void printLog(u8 level, const char *message);

void printHelpInfo();
