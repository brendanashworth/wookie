CC ?= clang
DBG ?= lldb
PREFIX ?= /usr/bin

# Compile for production
all:
	echo "Primary Make rule must be made by you (it is a framework, after all!)"
	echo "If you want to build the example, run: make example."

# Compile for development
dev: clean
	$(CC) -Wall -g src/wookie.c -o wookie
	$(DBG) wookie

example:
	$(CC) -Wall app/example/application.c -o wookie

# Clean it out
clean:
	rm -f wookie
