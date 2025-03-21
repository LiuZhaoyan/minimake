build: main.o utils.o
	gcc main.o utils.o -o app
main.o: main.c
	gcc -c main.c

utils.o: utils.ci
	gcc -c utils.c
