# General compilation settings
CC ?= clang
BIN ?= ./wookie
CFLAGS ?= -Wall -O3 -std=c99

# Settings specific for each rule
EXAMPLE_ENTRY ?= app/example/application.c

# EDIT THIS FOR YOUR OWN APP!
all:
	@echo "Primary Make rule must be made by you (it is a framework, after all!)"
	@echo "If you want to build the example, run: make example."

# Compiles the example program.
example:
	$(CC) $(CLFAGS) $(EXAMPLE_ENTRY) -o $(BIN)

# Clean it out
clean:
	rm -f $(BIN)
