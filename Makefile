# NOTE(brendan): set P using "export P=program_name" from shell
P=graphical_editor/graphical_editor

OBJECTS=
# STD_FLAGS=gnu11
STD_FLAGS=c90
MORE_FLAGS=#-DSHA1TEST
CRYPTO_IMPL=crypto-grabbag/Implementations
SLRE=slre
CFLAGS=-ggdb3 -std=$(STD_FLAGS) -Wall -Wextra -Werror -O0 -I$(CRYPTO_IMPL) -I$(SLRE)
LDLIBS=-lm -pthread -lssl -lcrypto
CC=gcc
CXX=g++

all: $(P)

$(P):
