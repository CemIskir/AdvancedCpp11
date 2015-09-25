#This Makefile is written by Cem ISKIR for CS 540 course, Asignment 1 (Containers)

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
LINK.o = $(CXX)

all:test

test:test.o

clean:
	rm *.o test
