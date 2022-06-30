
numgen: main.c
	mkdir -p bin
	gcc main.c -o bin/numgen -lpthread -s
