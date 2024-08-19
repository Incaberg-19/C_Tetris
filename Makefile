CC = gcc
CFLAGS = -std=c11 -lncurses -Wall -Werror -Wextra
SRC = $(shell ls brick_game/tetris/*.c | grep -v main.c)
MAIN = brick_game/tetris/main.c   
FRONT_SRC = ./gui/cli/*.c
TEST_SRC =  tests/test.c
LIB = s21_tetris.a
CHECKS_FLAGS = -lcheck -lm -lsubunit
GCOV_FLAGS = --coverage
GCOVR_PATH = gcovr
# GCOVR_PATH =usr/bin/gcovr
REPORT = report/index.html
DVI_DIRECTORY = doxygen/html/index.html

.PHONY: all gcov_report dvi 

all: $(LIB) start

s21_tetris.a:
	$(CC) $(CFLAGS) -c $(SRC) 
	ar rc $(LIB) *.o
	ranlib $(LIB)

install:
	$(CC) $(MAIN) $(SRC) $(FRONT_SRC) $(CFLAGS) -o tetris

uninstall:
	rm tetris

start: install
	./tetris

test:
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o test_result $(CHECKS_FLAGS)
	./test_result

test_gcov: 
	$(CC) $(CFLAGS) $(SRC) ${TEST_SRC} -o test_result $(CHECKS_FLAGS) $(GCOV_FLAGS)
	./test_result

gcov_report: test_gcov
	mkdir -p report
	${GCOVR_PATH} -r . --html --html-details -o $(REPORT) -e ${TEST_SRC}
	xdg-open $(REPORT)


valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test_result

dvi:
	doxygen
	cp tetris.png doxygen/html/
	xdg-open $(DVI_DIRECTORY)

dist:
	zip -r tetrisVS.zip .

clang:
	cp ../materials/linters/.clang-format brick_game/tetris/
	cp ../materials/linters/.clang-format gui/cli/
	cp ../materials/linters/.clang-format tests/

	clang-format -n brick_game/tetris/*.c
	clang-format -n gui/cli/*.c
	clang-format -n tests/*.c

	clang-format -i brick_game/tetris/*.c
	clang-format -i gui/cli/*.c
	clang-format -i tests/*.c

	rm brick_game/tetris/.clang-format
	rm gui/cli/.clang-format
	rm tests/.clang-format



clean:
	rm -rf *.zip
	rm -rf doxygen
	rm -rf score.txt
	rm -rf test_result
	rm -rf tetris
	rm -rf report
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.o
	rm -rf debug_result
	rm -rf $(LIB)
