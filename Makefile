#in bash:
export LD_LIBRARY_PATH=$(PWD)

all: gameoflife.o golcal.h golcal.o
	echo 'hello world!'
	gcc -Wall gameoflife.o golcal.o -o a -lmingw32 -lSDL2main -lSDL2

gameoflife.o: gameoflife.c
	gcc -Wall -c gameoflife.c -o gameoflife.o -lmingw32 -lSDL2main -lSDL2

golcal.o: golcal.c golcal.h
	gcc -Wall -c golcal.c -o golcal.o

#make clean
clean:
	rm -f a *.o