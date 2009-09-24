CCFLAGS=-I. -g 
LDFLAGS=-lm -g -lgmp

OBJECTS=matrix.o bezout.o

all: test generate benchmark

test: $(OBJECTS) test.o
	g++ -o test $(LDFLAGS) $(OBJECTS) test.o

generate: $(OBJECTS) generate.o
	g++ -o generate $(LDFLAGS) $(OBJECTS) generate.o

benchmark: $(OBJECTS) benchmark.o
	g++ -o benchmark $(LDFLAGS) $(OBJECTS) benchmark.o

%.o: %.cc
	g++ $(CCFLAGS) $< -c

clean:
	rm -f $(OBJECTS)
	rm -f test test.o
	rm -f generate generate.o
	rm -f benchmark benchmark.o
