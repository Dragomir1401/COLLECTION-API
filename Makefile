# Copyright Dragomir Andrei-Mihai
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# define targets
TARGETS=p1 p2 best

build: $(TARGETS)

out-p1: hashmap.c hashmap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra hashmap.c hashmap_out.c -o p1
	./p1

out-p2: treap.c treap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra treap.c treap_out.c -o p2
	./p2

out-best: treap.c treap_out.c
	$(CC) $(CFLAGS) -Wall -Wextra treap.c treap_out.c -o best
	./best

run-p1: hashmap.c hashmap_singular.c
	$(CC) $(CFLAGS) -Wall -Wextra hashmap.c hashmap_singular.c -o p1
	./p1

run-p2: treap.c treap_singular.c
	$(CC) $(CFLAGS) -Wall -Wextra treap.c treap_singular.c -o p2
	./p2

run-best: hashmap.c hashmap_singular.c
	$(CC) $(CFLAGS) -Wall -Wextra hashmap.c hashmap_singular.c -o best
	./best

pack:
	zip -FSr 312CA_Dragomir_Andrei_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean