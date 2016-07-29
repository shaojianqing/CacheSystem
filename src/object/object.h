#define TYPE_OBJECT_STRING 1


typedef struct CacheObject {

	int type;

	int encoding;

	Object *value;

} CacheObject;
