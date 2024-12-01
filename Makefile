CC=gcc
CFLAGS=-g -std=c11 -Wall -Wextra -Wpedantic -Werror=vla

SRC_DIR=days
BIN_DIR=bin

SOURCES=$(wildcard $(SRC_DIR)/*.c)
BINS=$(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

all: $(BINS)

run: $(BINS)
	./$(BIN_DIR)/01_one

clean:
	rm -rf $(BIN_DIR)

$(BINS): $(BIN_DIR) $(SOURCES)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/01_one $(SRC_DIR)/01_one.c

$(BIN_DIR):
	mkdir -p $@
