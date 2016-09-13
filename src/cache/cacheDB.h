
#define INIT_CACHE_CAPACITY			32

typedef struct CacheDB CacheDB;

struct CacheDB {
	
	HashMap *map;

	bool (*set)(CacheDB *this, char *key, Object *value);

	Object* (*get)(CacheDB *this, char *key);
};

CacheDB* createCacheDB();
