# Project Makefile: DHModule (Digital Humanities Module)
# Author: Dan McFalls

CC = g++

CFLAGS = -c -std=c++11 -O3 -Wall -pedantic

all: dh_test

dh_test: dh_test.o dh.o
	$(CC) dh_test.o dh.o -o dh_test

dh_test.o:
	$(CC) $(CFLAGS) dh_test.cc

dh.o: dh.cc dh.h
	$(CC) $(CFLAGS) dh.cc

clean:
	rm -f dh_test core *.o

.PHONY: clean all
