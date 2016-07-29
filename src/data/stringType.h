#include "../const/const.h"
#include "../type/type.h"

#define DEFALUT_STRING_SPACE		16

typedef struct String String;

typedef struct String {
	
	int length;

	int free;

	char *value;

	u32 (*getLength)(String *this);

	char* (*getValue)(String *this);

	String* (*subString)(String *this, int start, int end);

	String* (*catString)(String *this, String *string);

} String;

String* createString(char *value);

String* createStringWithRange(char *value, int range);
