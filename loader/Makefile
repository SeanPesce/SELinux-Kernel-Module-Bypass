# Author: Sean Pesce

CC=gcc
CFLAGS=-Wall -Werror -I.
FNAME = modload

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(FNAME).o
	$(CC) -o $(FNAME) $(FNAME).o

clean:
	rm -f $(FNAME).o $(FNAME)
