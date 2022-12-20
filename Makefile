# Copyright Dragomir Andrei-Mihai
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# define targets
TARGETS=p1 p2 best

build: $(TARGETS)

run-p1: hashmap.c hashmap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra hashmap.c hashmap_out.c -o p1
	./p1

run-p2: treap.c treap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra treap.c treap_out.c -o p2
	./p2

run-best: treap.c treap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra treap.c treap_out.c -o best
	./best

pack:
	zip -FSr 312CA_Dragomir_Andrei_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean