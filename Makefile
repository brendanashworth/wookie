# General compilation settings
CC ?= clang
BIN ?= ./wookie
CFLAGS ?= -Wall -Wextra -O2 -Ideps -Isrc

# If we're compiling on GCC...
ifeq ($(CC), gcc)
	CFLAGS += -std=c99 -lpthread
endif

# Settings specific for each rule
EXAMPLE_ENTRY ?= app/example/application.c

# EDIT THIS FOR YOUR OWN APP!
all:
	@echo "Primary Make rule must be made by you (it is a framework, after all!)"
	@echo "If you want to build the example, run: make example."

# Compiles the example program.
example:
	$(CC) $(CFLAGS) $(EXAMPLE_ENTRY) -o $(BIN)

# Clean it out
clean:
	rm -f $(BIN)

.PHONY: app benches deps src test