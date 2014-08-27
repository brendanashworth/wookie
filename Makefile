CC ?= clang

all:
	$(CC) -g src/wookie.c -o wookie

clean:
	rm -f wookie