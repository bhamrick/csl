CCFLAGS=-I. -g -O2
LDFLAGS=-lm -g -lgmp -lglut -lGLU -lGLU -lXmu -lXext -lXi -lX11 -O2

OBJECTS=matrix.o bezout.o

all: test generate benchmark display surfacehom

test: $(OBJECTS) test.o
	g++ -o test $(LDFLAGS) $(OBJECTS) test.o

generate: $(OBJECTS) generate.o
	g++ -o generate $(LDFLAGS) $(OBJECTS) generate.o

benchmark: $(OBJECTS) benchmark.o
	g++ -o benchmark $(LDFLAGS) $(OBJECTS) benchmark.o

display: display.o
	g++ -o display $(LDFLAGS) display.o

surfacehom: surfacehom.o
	g++ -o surfacehom $(LDFLAGS) surfacehom.o

%.o: %.cc
	g++ $(CCFLAGS) $< -c

clean:
	rm -f $(OBJECTS)
	rm -f test test.o
	rm -f generate generate.o
	rm -f benchmark benchmark.o
	rm -f display display.o
	rm -f surfacehom surfacehom.o
