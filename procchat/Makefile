CC=gcc
CFLAGS=-Wall -Werror -Wvla -std=c11 -fsanitize=address
PFLAGS=-fprofile-arcs -ftest-coverage
DFLAGS=-g
HEADERS=server.h
SRC=server.c

procchat: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -std=gnu99 -o $@

test:
	chmod u+x test.sh clients/client1 clients/client2 clients/client3 clients/client4 clients/client5
	make procchat
	./test.sh

clean:
	rm -f procchat

