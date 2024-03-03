CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -lm
OBJS = symnmf.o

symnmf: $(OBJS)
	$(CC) -o symnmf $(OBJS) $(CFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) -c symnmf.c $(CFLAGS)

clean:
	rm -f *.o symnmf
