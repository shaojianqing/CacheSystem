#include <string.h>

#include "../const/const.h"
#include "../type/type.h"
#include "dataType.h"

int StringHashCode(Object *string) {
	char *value = string;
	if (value!=NULL && *value!='\0') {
		int hashCode = 0;
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
