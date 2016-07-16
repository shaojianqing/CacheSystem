#include <stdio.h>

#include "data/hashMap.h"
#include "cacheServer.h"

int main(int argc, char **argv) {
	printf("The CacheSystem Server has been started^_^\n");

	HashMap *map = createHashMap(StringHashCode, StringEqualFun, 32);
	map->put(map, "name", "shaojianqing");
	map->put(map, "gender", "male");
	map->put(map, "birthday", "1989-02-08");
	map->put(map, "job", "software engineer");
	map->put(map, "title", "senior software engineer");
	
	map->printHashMap(map); 	

	printf("\n");
	printf("The Name: %s\n", (char *)map->get(map, "name"));
	printf("The Gender: %s\n", (char *)map->get(map, "gender"));
	printf("The Birthday: %s\n", (char *)map->get(map, "birthday"));
	printf("The Job: %s\n", (char *)map->get(map, "job"));
	printf("The Title: %s\n", (char *)map->get(map, "title"));
	printf("\n");
}
