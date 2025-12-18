CC=g++
CFLAGS=-std=gnu++17 -O0 -g3 -Wall -Wextra -Wpedantic
LFLAGS=-lm -ldl -lSDL2
SOURCE_FILES=BoringPartycles.cpp
BINARY=thingy

all: debug

debug:	${SOURCE_FILES}
	${CC} ${CFLAGS} -o ${BINARY} ${SOURCE_FILES} ${LFLAGS}
