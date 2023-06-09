# -*- mode: makefile -*-

PREFIX ?= /usr/local
SRC = .

CFLAGS = -Wall -Werror -std=c99 -pedantic -ggdb
LFLAGS =
LDFLAGS =

# Uncomment the following if target is a Shared library
CFLAGS += -fPIC -I/usr/include/bash -I/usr/include/bash/include
LFLAGS += --shared

# Build module list (info make -> "Functions" -> "File Name Functions")
MODULES = $(addsuffix .o,$(basename $(wildcard $(SRC)/*.c)))
ROOTS = $(filter-out ./utilities, $(basename $(wildcard $(SRC)/*.c)))
BUILTINS = $(addsuffix .so, $(ROOTS))

# Libraries need header files.  Set the following accordingly:
HEADERS =

# Declare non-filename targets
.PHONY: all install uninstall clean help

all: $(BUILTINS)

%so : %o
	$(CC) $(LFLAGS) -o $@ $< $(LDFLAGS)

%o : %c
	$(CC) $(CFLAGS) -c -o $@ $<

test:
	@echo $(BUILTINS)

clean:
	rm -f *.so
	rm -f $(MODULES)

help:
	@echo "Makefile options:"
	@echo
	@echo "  clean      to remove generated files"
	@echo "  help       this display"
