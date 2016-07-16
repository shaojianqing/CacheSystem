#include "../const/const.h"
#include "../type/type.h"

#define LIMIT_FACTOR 0.75

#define STATUS_SUCCESS	1
#define STATUS_FAILURE  0

typedef u32 (*HashCode)(Object *key);

typedef bool (*EqualFun)(Object *obj1, Object *obj2);

typedef struct Entry Entry;

typedef struct Entry {
	
	Object *key;
	
	Object *value;

	Entry *next;	

} Entry;

typedef struct HashMap HashMap;

typedef struct HashMap {
	
	Entry **table;

	u32 count;

	u32 limit;

	u32 capacity;

	u32 (*put)(HashMap *this, Object *key, Object *value);

	Object* (*get)(HashMap *this, Object *key);

	u32 (*getCount)(HashMap *this);

	u32 (*remove)(HashMap *this, Object *key);

	bool (*containsKey)(HashMap *this, Object *key);

	void (*printHashMap)(HashMap *this);

	HashCode hashCode;

	EqualFun equalFun;
	
} HashMap;

HashMap* createHashMap(HashCode hashCode, EqualFun equalFun, u32 capacity);

u32 StringHashCode(Object *string);

bool StringEqualFun(Object *obj1, Object *obj2);
