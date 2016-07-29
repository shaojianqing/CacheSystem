#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../type/type.h"
#include "../data/stringType.h"
#include "object.h"

CacheObject *createObject(int type, Object *value) {
	if (value!=NULL) {
		CacheObject *object = (CacheObject *)malloc(sizeof(CacheObject));
		if (object!=NULL) {
			object->type = type;
			object->value = value;
			return object;		
		}
	}
	return NULL;
}

CacheObject *createStringObject(String *value) {
	if (value!=NULL) {
		CacheObject *object = (CacheObject *)malloc(sizeof(CacheObject));
		if (object!=NULL) {
			object->type = TYPE_OBJECT_STRING;
			object->value = value;
			return object;		
		}
	}
	return NULL;
}
