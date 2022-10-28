#in bash:
export LD_LIBRARY_PATH=$(PWD)

all: gameoflife.o golcal.h golcal.o
	echo 'hello world!'
	gcc -Wall gameoflife.o golcal.o -o a -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

gameoflife.o: gameoflife.c
	gcc -Wall -c gameoflife.c -o gameoflife.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

golcal.o: golcal.c golcal.h
	gcc -Wall -c golcal.c -o golcal.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

#make clean
clean:
	rm -f a *.o