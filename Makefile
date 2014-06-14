# Makefile; Lets not make this difficult, shall we 

CXXFLAGS=-std=c++11 -O0 -g -Wall -pedantic


2048: game.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

game.o: game.h
main.o: game.h debug.h bbqbot.h

all: 2048

clean:
	-rm 2048 *.o

.SUFFIXES:
.SUFFIXES: .cc .o .h
