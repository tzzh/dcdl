.PHONY: clean build

SRC = jeanmarc.c
BIN_DIR = bin

clean:
	rm -rf $(BIN_DIR)

build: clean
	mkdir $(BIN_DIR)
	gcc -Wall -std=c99 -pedantic $(SRC) -o $(BIN_DIR)/jeanmarc

