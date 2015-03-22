# Makefile; Lets not make this difficult, shall we 

CXXFLAGS=-std=c++11 -O0 -g -ggdb -Wall -pedantic -DDEBUG_CMDS


2048: game.o main.o cvar.o glob.o q_trie.o prompt.o cmds.o
	$(CXX) $(CXXFLAGS) -lao -o $@ $^ 

game.o: game.h
main.o: game.h display.h debug.h bbqbot.h
cvar.o: cvar.h 
glob.o: glob.h
q_trie.o: q_trie.h
prompt.o: prompt.h
cmds.o: cmds.h

all: 2048

clean:
	-rm 2048 *.o

.SUFFIXES:
.SUFFIXES: .c .cc .o .h
