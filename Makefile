CCFLAGS=-I. -g -O2
LDFLAGS=-lm -g -lgmp -lglut -lGLU -lGLU -lXmu -lXext -lXi -lX11 -lcurses -O2

OBJECTS=matrix.o bezout.o

all: test generate benchmark display surfacehom classify onedim increment maketorus makeklein compute createimages inchom.o

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

classify: classify.o
	g++ -o classify $(LDFLAGS) classify.o

onedim: onedim.o
	g++ -o onedim $(LDFLAGS) onedim.o

increment: increment.o bezout.o
	g++ -o increment $(LDFLAGS) increment.o bezout.o

maketorus: maketorus.o
	g++ -o maketorus $(LDFLAGS) maketorus.o

makeklein: makeklein.o
	g++ -o makeklein $(LDFLAGS) makeklein.o

compute: compute.o inchom.o bezout.o
	g++ -o compute $(LDFLAGS) compute.o inchom.o bezout.o

createimages: createimages.o inchom.o bezout.o
	g++ -o createimages $(LDFLAGS) createimages.o inchom.o bezout.o

%.o: %.cc
	g++ $(CCFLAGS) $< -c

clean:
	rm -f $(OBJECTS)
	rm -f test test.o
	rm -f generate generate.o
	rm -f benchmark benchmark.o
	rm -f display display.o
	rm -f surfacehom surfacehom.o
	rm -f classify classify.o
	rm -f onedim onedim.o
	rm -f increment increment.o
	rm -f maketorus maketorus.o
	rm -f makeklein makeklein.o
	rm -f compute compute.o
	rm -f createimages createimages.o
	rm -f inchom.o
