#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "hashMap.h"

static u32 put(HashMap *this, Object *key, Object *value);

static Object* get(HashMap *this, Object *key);

static u32 getCount(HashMap *this);

static u32 reHash(HashMap *this);

static void printHashMap(HashMap *this);

static u32 removeByKey(HashMap *this, Object *key);

static bool containsKey(HashMap *this, Object *key);

static u32 putInner(HashMap *this, Object *key, Object *value);

HashMap* createHashMap(HashCode hashCode, EqualFun equalFun, u32 capacity) {
	if (hashCode!=NULL && capacity>0) {
		HashMap *map = (HashMap *)malloc(sizeof(HashMap));
		if (map!=NULL) {
			map->count = 0;
			map->capacity = capacity;
			map->table = malloc(sizeof(Entry *)*capacity);		
			map->limit = capacity*LIMIT_FACTOR;
			map->printHashMap = printHashMap;
			map->containsKey = containsKey;
			map->remove = removeByKey;
			map->hashCode = hashCode;
			map->equalFun = equalFun;
			map->getCount = getCount;
			map->put = put;
			map->get = get;			
		}
		return map;
	} else {
		return NULL;	
	}
}

static u32 put(HashMap *this, Object *key, Object *value) {
	if (this!=NULL && key!=NULL && value!=NULL) {
		if (this->count >= this->limit) {
			if (reHash(this)==STATUS_SUCCESS) {
				if (putInner(this, key, value)) {
					this->count++;
					return STATUS_SUCCESS;				
				}
			}		
		} else {
			if (putInner(this, key, value)) {
				this->count++;
				return STATUS_SUCCESS;				
			}	
		}		
	}
	return STATUS_FAILURE;
}

static Object* get(HashMap *this, Object *key) {
	if (this!=NULL && key!=NULL) {
		u32 hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *entry = this->table[index];
		while(entry!=NULL) {
			if (this->equalFun(entry->key, key)) {
				return entry->value;			
			}
			entry = entry->next;		
		}
	} else {
		return NULL;
	}
}

static u32 getCount(HashMap *this) {
	if (this!=NULL) {
		return this->count;	
	} else {
		return 0;
	}
}

static u32 removeByKey(HashMap *this, Object *key) {
	return STATUS_SUCCESS;
}

static bool containsKey(HashMap *this, Object *key) {
	return true;
}

static u32 reHash(HashMap *this) {
	return STATUS_SUCCESS;
}

static u32 putInner(HashMap *this, Object *key, Object *value) {
	u32 hashCode = this->hashCode(key);
	u32 index = (hashCode & (this->capacity-1));
	Entry *old = this->table[index];
	Entry *entry = (Entry *)malloc(sizeof(Entry));
	entry->key = key;
	entry->value = value;
	entry->next = old;
	this->table[index] = entry;
	return STATUS_SUCCESS;
}

static void printHashMap(HashMap *this) {
	if (this!=NULL) {
		u32 i=0;
		for (i=0;i<this->capacity;++i) {
			printf("Index:%d ", i);
			Entry *entry = this->table[i];
			while (entry!=NULL) {
				printf("(%s, %s)->", (char *)entry->key, (char *)entry->value);
				entry = entry->next;			
			}
			printf("\n");
		}
	}
}

u32 StringHashCode(Object *string) {
	char *value = string;
	if (value!=NULL && *value!='\0') {
		u32 hashCode = 0;
		while (*value!='\0') {
			hashCode = hashCode*31 + (*value);
			value++;
		}	
		return hashCode;
	} else {
		return 0;	
	}
}

bool StringEqualFun(Object *obj1, Object *obj2) {
	if (obj1!=NULL && obj2!=NULL) {
		if (obj1==obj2) {
			return true;		
		} else {
			char *str1 = obj1;
			char *str2 = obj2;
			return (strcmp(str1, str2)==0);
		}
	} else if (obj1==NULL && obj2==NULL) {
		return true;
	} else {
		return false;	
	}
}

