CC ?= clang
DBG ?= lldb
PREFIX ?= /usr/bin

# Compile for production
all: clean
	$(CC) -O3 src/wookie.c -o wookie

# Compile for development
dev: clean
	$(CC) -Wall -g src/wookie.c -o wookie
	$(DBG) wookie

example:
	$(CC) -Wall app/example/application.c -o wookie

# Install it globally (oh god...)
install: clean all
	mv ./wookie $(PREFIX)/wookie

# Clean it out
clean:
	rm -f wookie
