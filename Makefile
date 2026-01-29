all: main

SANITIZE=
# SANITIZE=-fsanitize=undefined
# SANITIZE=-fsanitize=address
# CFLAGS=-W -Wall -Wextra -O3 ${SANITIZE}
CFLAGS=-W -Wall -Wextra -g

main:
	${CC} ${CFLAGS} src/main.c -o main -Llib -Iinclude/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
