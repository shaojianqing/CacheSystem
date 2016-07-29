#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../const/const.h"
#include "../type/type.h"
#include "../data/dataType.h"
#include "../data/hashMap.h"
#include "cacheDB.h"

static bool set(CacheDB *this, char *key, Object *value);

static Object* get(CacheDB *this, char *key);

CacheDB *createCacheDB() {
	CacheDB *cacheDB = (CacheDB *)malloc(sizeof(CacheDB));
	if (cacheDB!=NULL) {
		cacheDB->map = createHashMap(StringHashCode, StringEqualFun, INIT_CACHE_CAPACITY);
		cacheDB->set = set;
		cacheDB->get = get;
		return cacheDB;
	}
	return NULL;
}

static bool set(CacheDB *this, char *key, Object *value) {
	if (this!=NULL && key!=NULL && value!=NULL) {
		HashMap *map = this->map;
		map->put(map, key, value);
		return true;
	}
	return false;
}

static Object* get(CacheDB *this, char *key) {
	if (this!=NULL && key!=NULL) {
		HashMap *map = this->map;
		return map->get(map, key);
	}
	return NULL;
}
