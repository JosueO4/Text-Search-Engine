FILES = main.c

all:
	gcc -std=c99 -Wall -Wextra -pedantic main.c -o main.out

run:
	./main.out

clean:
	rm *.out
