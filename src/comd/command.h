
typedef void (*CommandProcess)(Client *client);

typedef struct CacheCommand {
	
	char *name;

	CommandProcess commandProcess;

} CacheCommand;

void executeCommand(Client *client);
