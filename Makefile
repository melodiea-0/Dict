CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Wno-override-init -std=c11 -I./include --ms-extensions
SRC = ./src/
TESTS = ./tests/

.PHONY: all tests-list leaks-list clean

all: leaks-list leaks-keyval

tests-keyval:
	$(CC) $(CFLAGS) $(SRC)keyval.c $(TESTS)keyval.c -o tests_keyval -lcheck -lm -lsubunit

test-keyval: tests-keyval
	./tests_keyval

leaks-keyval: tests-keyval
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tests_keyval

tests-list:
	$(CC) $(CFLAGS) $(SRC)list.c $(TESTS)list.c -o tests_list -lcheck -lm -lsubunit

test-list: tests-list
	./tests_list

leaks-list: tests-list
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tests_list

clean:
	rm -f tests_list 