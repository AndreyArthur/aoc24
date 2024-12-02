CC=gcc
CFLAGS=-g -std=c11 -Wall -Wextra -Wpedantic -Werror=vla

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=days
LIB_DIR=lib

LIB_SOURCES=$(wildcard $(LIB_DIR)/*.c)
LIB_OBJECTS=$(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SOURCES))

SOURCES=$(wildcard $(SRC_DIR)/*.c)
BINS=$(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

all: $(BINS)

run: $(BINS)
	./$(BIN_DIR)/01_one

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

$(BINS): $(BIN_DIR) $(SOURCES) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/01_one $(SRC_DIR)/01_one.c $(LIB_OBJECTS)

$(LIB_OBJECTS): $(OBJ_DIR) $(LIB_SOURCES)
	$(CC) $(CFLAGS) -c $(LIB_DIR)/io.c -o $(OBJ_DIR)/io.o

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@
