#######################################
#    Makefile for CacheSystem	      #
#######################################

#   The general compiling commands    #

CP = cp
CC = gcc
SUDO = sudo
CCFLAGES = -c 

TARGET = cacheServer

TGT = tgt/*

OBJS = tgt/cacheServer.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/config.o tgt/event.o tgt/client.o

.PHONY : build clean

build : clean $(TARGET)

clean :
	 rm -f $(TARGET) $(TGT)

tgt/client.o : src/client/client.c src/client/client.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/event.o	: src/event/event.c src/event/event.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/config.o : src/config/config.c src/config/config.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/network.o : src/inet/network.c src/inet/network.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/dataType.o : src/data/dataType.c src/data/dataType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/arrayList.o : src/data/arrayList.c src/data/arrayList.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/hashMap.o : src/data/hashMap.c src/data/hashMap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/cacheServer.o : src/cacheServer.c src/cacheServer.h
	$(CC) $(CCFLAGES) $< -o $@

cacheServer : tgt/cacheServer.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/config.o tgt/event.o tgt/client.o
	$(CC) $(OBJS) -o $@


