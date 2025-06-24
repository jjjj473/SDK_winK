CC=gcc
CXX=g++
CFLAGS=-Wall -O2 -fPIC
CXXFLAGS=-Wall -O2 -fPIC

all: libwink.a

wink_sdk.o: wink_sdk.c
	$(CC) $(CFLAGS) -c $< -o $@

wink_errors.o: wink_errors.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

libwink.a: wink_sdk.o wink_errors.o
	ar rcs $@ $^

clean:
	rm -f *.o libwink.a
