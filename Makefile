all: main

SANITIZE=
# SANITIZE=-fsanitize=undefined
# SANITIZE=-fsanitize=address
# CFLAGS=-W -Wall -Wextra -O3 ${SANITIZE}
CFLAGS=-W -Wall -Wextra -g

LIBS=-lopensimplexnoise -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
DEPS=

BUILD_DIR := build

main:
	${CC} ${CFLAGS} -c src/hud.c -o hud.o -Iinclude/
	${CC} ${CFLAGS} -c src/player.c -o player.o -Iinclude/
	${CC} ${CFLAGS} -c src/world.c -o world.o -Iinclude/
	${CC} ${CFLAGS} world.o player.o hud.o src/main.c  -o main -Llib -Iinclude/ $(LIBS)

# TODO fix
.PHONY: main clean

clean:
	rm -f $(BUILD_DIR)/*.o

