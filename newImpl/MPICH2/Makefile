OBJS:=$(shell ls *.cpp | sed 's/.cpp//g')
CC=mpic++

default: compile

all: compile

compile: $(OBJS)

%: %.cpp
	${CC} -std=c++11 -O2 -o $@ $<

test: compile
	./testme.sh

clean:
	rm -rf *.o a.out $(OBJS)
