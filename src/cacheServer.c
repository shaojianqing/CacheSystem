#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type/type.h"
#include "data/dataType.h"
#include "data/hashMap.h"
#include "data/arrayList.h"
#include "inet/network.h"
#include "cacheServer.h"

int main(int argc, char **argv) {

	printf("Cache Service is now running^_^");
		
	startService();
}
