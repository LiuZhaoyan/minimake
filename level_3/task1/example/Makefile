# 目标列表
all: build test

build: main.o utils.o
gcc main.o utils.o -o app

main.o: main.c
gcc -c main.c

utils.o: utils.c
gcc -c utils.c

