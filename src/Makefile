CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)
TEST_FILE_NAME = tests.c

all:
	@gcc $(SRC)
	@./a.out

clean:
	@rm -rf *.o *.a *.so *.gcda *.gcno *.gch *.dSYM rep.info *.tar *.html *.css *.out test report *.txt test.info test.dSYM

