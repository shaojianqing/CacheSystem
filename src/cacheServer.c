#include <stdio.h>

#include "data/dataType.h"
#include "data/hashMap.h"
#include "data/arrayList.h"
#include "cacheServer.h"

int main(int argc, char **argv) {
	printf("The CacheSystem Server has been started^_^\n");

	HashMap *map = createHashMap(StringHashCode, StringEqualFun, 32);	
	
	map->put(map, "name", "shaojianqing");
	map->put(map, "gender", "male");
	map->put(map, "birthday", "1989-02-08");
	map->put(map, "job", "software engineer");
	map->put(map, "title", "senior software engineer");

	char key[] = {'n','a', 'm', 'e', '\0'};

	printf("\n");
	printf("The Name: %s\n", (char *)map->get(map, key));
	printf("The Gender: %s\n", (char *)map->get(map, "gender"));
	printf("The Birthday: %s\n", (char *)map->get(map, "birthday"));
	printf("The Job: %s\n", (char *)map->get(map, "job"));
	printf("The Title: %s\n", (char *)map->get(map, "title"));
	printf("\n");

	if (map->containsKey(map, "job")) {
		printf("Yes it exists:%d\n", map->getCount(map));
		map->remove(map, "job");
		printf("The Job: %s\n", (char *)map->get(map, "job"));
		printf("Yes it exists:%d\n", map->getCount(map));	
	}

	map->printHashMap(map);

	destroyHashMap(map);

	ArrayList *list = createArrayList(StringEqualFun, 32);
	list->add(list, "shaojianqing");
	list->add(list, "male");
	list->add(list, "1989-02-08");
	list->add(list, "software engineer");
	list->add(list, "senior software engineer");

	u32 i=0;
	for (i=0;i<list->getSize(list);++i) {
		printf("The list item is: %s \n", (char *)list->get(list, i));	
	}

	list->remove(list, "1989-02-08");
	printf("\n\n");
	
	for (i=0;i<list->getSize(list);++i) {
		printf("The removed list item is: %s \n", (char *)list->get(list, i));	
	}

	destroyArrayList(list);
}
