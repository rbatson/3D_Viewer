CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)
TEST_FILE_NAME = tests.c

all:
	@$(CC) $(CFLAGS) $(SRC) -o s21_3D
	@./s21_3D
	@rm s21_3D

clean:
	@rm -rf *.o *.a *.so *.gcda *.gcno *.gch *.dSYM rep.info *.tar *.html *.css *.out test report *.txt test.info test.dSYM

