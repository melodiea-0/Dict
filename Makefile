CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -Wpedantic -std=c11 -I./include
SRC = ./src/
TESTS = ./tests/

.PHONY: all tests-list leaks-list clean

all: leaks-list

tests-list:
	$(CC) $(CFLAGS) $(SRC)list.c $(TESTS)list.c -o tests_list -lcheck -lm -lsubunit
	./tests_list

leaks-list: tests-list
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tests_list

clean:
	rm -f tests_list 