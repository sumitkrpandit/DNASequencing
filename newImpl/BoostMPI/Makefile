OBJS:=$(shell ls *.cpp | sed 's/.cpp//g')
CC=mpicxx

default: compile

all: compile

compile: $(OBJS)

%: %.cpp
	${CC} -std=c++11 -O2 -I ~/usr/include -o $@ $< ~/usr/lib/libboost_mpi.a ~/usr/lib/libboost_serialization.a

test: compile
	./testme.sh

clean:
	rm -rf *.o *.dat *.txt a.out $(OBJS)
