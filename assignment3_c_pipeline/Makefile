
CC=gcc

# The line with -DDEBUG can be used for development. When
# building your code for evaluation, however, the line *without*
# the -DDEBUG will be used.
#
# please note extra file addes (functions)
#

CFLAGS=-c -Wall -g -D_GNU_SOURCE -std=c99 -O0 


all: song_analyzer

song_analyzer: song_analyzer.o list.o emalloc.o functions.o
	$(CC) song_analyzer.o list.o emalloc.o functions.o -o song_analyzer

song_analyzer.o: song_analyzer.c list.h emalloc.h functions.h
	$(CC) $(CFLAGS) song_analyzer.c

list.o: list.c list.h emalloc.h
	$(CC) $(CFLAGS) list.c

emalloc.o: emalloc.c emalloc.h
	$(CC) $(CFLAGS) emalloc.c

functions.o: functions.c functions.h emalloc.h list.h
	$(CC) $(CFLAGS) functions.c

clean:
	rm -rf *.o song_analyzer
