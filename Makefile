# Makefile for A09

CFLAGS= -Wall -Wpedantic -fsigned-char

all: a09 

a09.o: a09.h 

clean:
	-$(RM) a09

INSTALL := install
INSTALL_PROGRAM := $(INSTALL)
prefix := /usr/local
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin

install: a09
	$(INSTALL_PROGRAM) a09 $(bindir)/a09
