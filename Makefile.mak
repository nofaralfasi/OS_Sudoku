TARGET = v1 v2
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

defult: $(TARGET)
all: defult

OBJECTSA = MainA.c multiThreads.c
OBJECTSB = MainB.c mutexMultiThreads.c
HEADERS = multiThreads.h mutexMultiThreads.h

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET)$(OBJECTSA)$(OBJECTSB)
	
v1: $(OBJECTSA)
	$(CC) $(OBJECTSA) -Wall -pthread $(LIBS) -o $@ 

v2: $(OBJECTSB)
	$(CC) $(OBJECTSB) -Wall -pthread $(LIBS) -o $@ 
	
clean:
	-rm -f *.o
	-rm -f v1
	-rm -f v2