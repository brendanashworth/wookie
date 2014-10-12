# Simple Makefile for benches
# Run `make (file)` to make it, i.e. `make pthread`
# It'll compile to pthread.o

CC ?= gcc

pthread:
	$(CC) pthread.c -o pthread.o

http_parser:
	$(CC) http_parser.c -o http_parser.o

cluster:
	$(CC) cluster.c -o cluster.o

clean:
	rm -f ./*.o