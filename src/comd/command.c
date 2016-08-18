#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../type/type.h"
#include "../data/hashMap.h"
#include "../cache/cacheDB.h"
#include "../data/stringType.h"
#include "../client/client.h"
#include "../config/config.h"
#include "command.h"

char sendBuffer[1024];

extern Server server;

extern ReplyInfo replyInfo;;

static void setCommand(Client *client);

static void getCommand(Client *client);

static CacheCommand* lookupCommand(char *name);

static CacheCommand commandTable[] = {
				{"get", getCommand},
				{"set", setCommand},
				{NULL,NULL}};

void executeCommand(Client *client) {
	if (client!=NULL && client->paramNum>0) {
		String *nameStr = client->params[0];
		char *name = nameStr->value;
		CacheCommand *command = lookupCommand(name);
		if (command!=NULL) {
			command->commandProcess(client);	
		}
	}
}

static CacheCommand* lookupCommand(char *name) {
	u32 i=0;
	CacheCommand *command = &(commandTable[i]); 
	while (command->name!=NULL && command->commandProcess!=NULL) {
		if (strcmp(command->name, name)==0) {
			return command;
		} else {
			command = &commandTable[++i]; 	
		}
	}
	return NULL;
}

static void setCommand(Client *client) {
	if (client->paramNum==3) {
		CacheDB *cacheDB = server.cacheDB;
		String *keyStr = client->params[1];
		String *valStr = client->params[2];
		cacheDB->set(cacheDB, keyStr->value, valStr->value);
		HashMap *map = cacheDB->map;
		map->printHashMap(map);
		client->replyToClient(client, replyInfo.ok);
	} else {
		String *info = createString("-ERR wrong number of arguments for SET\r\n");
		client->replyToClient(client, info);
	}
}

static void getCommand(Client *client) {
	CacheDB *cacheDB = server.cacheDB;
	String *keyStr = client->params[1];
	char *value = cacheDB->get(cacheDB, keyStr->value);
	memset(sendBuffer, 0, sizeof(sendBuffer));
	if (value!=NULL) {			
		sprintf(sendBuffer, "$%ld\r\n%s\r\n", strlen(value), value);
	} else {
		sprintf(sendBuffer, "$-1\r\n");	
	}

	String *result = createString(sendBuffer);
	client->replyToClient(client, result);
}
