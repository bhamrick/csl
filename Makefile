CCFLAGS=-I. -g 
LDFLAGS=-lm -g -lgmp

OBJECTS=matrix.o bezout.o

all: test generate

test: $(OBJECTS) test.o
	g++ -o test $(LDFLAGS) $(OBJECTS) test.o

generate: $(OBJECTS) generate.o
	g++ -o generate $(LDFLAGS) $(OBJECTS) generate.o

%.o: %.cc
	g++ $(CCFLAGS) $< -c

clean:
	rm -f $(OBJECTS)
	rm -f a.out
	rm -f test
	rm -f generate
	rm -f test.o
	rm -f generate.o
