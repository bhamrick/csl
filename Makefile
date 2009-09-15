CCFLAGS=-I. -g
LDFLAGS=-lm -g

OBJECTS=matrix.o bezout.o test.o

all: $(OBJECTS)
	g++ $(LDFLAGS) $(OBJECTS)

%.o: %.cc
	g++ $(CCFLAGS) $< -c

clean:
	rm -f $(OBJECTS)
	rm -f a.out
