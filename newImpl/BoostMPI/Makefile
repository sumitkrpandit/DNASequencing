OBJS:=$(shell ls *.cpp | sed 's/.cpp//g')
CC=mpic++

default: compile

all: compile

compile: $(OBJS)

%: %.cpp
	${CC} -std=c++11 -O2 -o $@ $< /usr/lib/x86_64-linux-gnu/libboost_mpi.a /usr/lib/x86_64-linux-gnu/libboost_serialization.a

test: compile
	./testme.sh

clean:
	rm -rf *.o a.out $(OBJS)
