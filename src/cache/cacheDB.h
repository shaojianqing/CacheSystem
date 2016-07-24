
typedef struct CacheDB {
	
	HashMap *map;

	int (*set)(const char *key, Object *value);

	Object* (*get)(const char *key);	
}

CacheDB *createCacheDB();
