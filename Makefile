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

# Contortions to install collection of man pages
MANPAGES := $(wildcard bashbi*.7)
define newline



endef
define MAKEMAN
	soelim -r $(1) | gzip -c - > $(PREFIX)/share/man/man7/$(1).gz $(newline)
endef
# end of man page contortions

# Declare non-filename targets
.PHONY: all install uninstall clean help

all: $(BUILTINS)

%so : %o
	$(CC) $(LFLAGS) -o $@ $< $(LDFLAGS)

%o : %c
	$(CC) $(CFLAGS) -c -o $@ $<

bashbi*.7:
	echo "Making man page $@"

install:
	mkdir --mode=755 -p $(PREFIX)/share/man/man7
	$(foreach file, $(MANPAGES), $(call MAKEMAN,$(file)))

uninstall:
	rm -f $(PREFIX)/share/man/man7/bashbi*.7.gz

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
