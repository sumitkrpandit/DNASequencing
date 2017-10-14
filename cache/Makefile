default: seq

all: seq

clean:
	rm DNASequencingSerialCache.o seq.out

DNASequencingSerialCache.o: Cache.h DNASequencingSerialCache.cpp
	g++ -std=c++11 -Wall -c DNASequencingSerialCache.cpp

seq: DNASequencingSerialCache.o
	g++ -std=c++11 -Wall DNASequencingSerialCache.o -o seq.out
