
#define INIT_CACHE_CAPACITY			32

typedef struct HashMap HashMap;

typedef struct CacheDB CacheDB;

typedef struct CacheDB {
	
	HashMap *map;

	bool (*set)(CacheDB *this, char *key, Object *value);

	Object* (*get)(CacheDB *this, char *key);	

} CacheDB;

CacheDB* createCacheDB();
