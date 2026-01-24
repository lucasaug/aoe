all:
	gcc src/main.c -o main -Llib -Iinclude/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
