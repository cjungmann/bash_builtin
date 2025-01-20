TARGET_ROOT = TEMPLATE
TARGET = $(TARGET_ROOT)
BUILTIN = $(TARGET_ROOT)

PREFIX ?= /usr/local

# Change if source files not in base directory:
SRC = .

# Configure output to be a shared library:
CFLAGS = -Wall -Werror -std=c99 -pedantic -ggdb -fPIC
LFLAGS = --shared
# Add dependency libraries here:
LDFLAGS = -largeater

CFLAGS += -I/usr/include/bash -I/usr/include/bash/include

# Build module list (info make -> "Functions" -> "File Name Functions")
MODULES = $(addsuffix .o,$(basename $(wildcard $(SRC)/*.c)))

HEADERS = $(wildcard $(SRC)/*.h)

# Declare non-filename targets
.PHONY: all install uninstall clean help

all: $(TARGET)

$(TARGET) : $(MODULES)
	$(CC) $(LFLAGS) -o $@ $(MODULES) $(LDFLAGS)

*.c: $(HEADERS)
	@echo "Forcing full recompile after any header file change"
	touch *.c

%o: %c
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	install -D --mode=775 $(TARGET) -t $(PREFIX)/lib/bash
	mkdir --mode=755 -p $(PREFIX)/share/man/man1
	soelim $(TARGET_ROOT).1 | gzip -c - > $(PREFIX)/share/man/man1/$(TARGET_ROOT).1.gz
	mkdir --mode=755 -p $(PREFIX)/share/man/man7
	soelim $(TARGET_ROOT).7 | gzip -c - > $(PREFIX)/share/man/man7/$(TARGET_ROOT).7.gz

uninstall:
	rm -f $(PREFIX)/lib/bash/$(TARGET)
	rm -f $(PREFIX)/share/man/man1/$(TARGET_ROOT).1.gz
	rm -f $(PREFIX)/share/man/man7/$(TARGET_ROOT).7.gz

clean:
	rm -f $(TARGET)
	rm -f $(MODULES)

help:
	@echo "Makefile options:"
	@echo
	@echo "  install    to install project"
	@echo "  uninstall  to uninstall project"
	@echo "  clean      to remove generated files"
	@echo "  help       this display"
