PROJECT := "flexiname"
VERSION := "git"
DEBUG := 1

CFLAGS := -ansi -O2 -pipe -DVERSION=$(VERSION)
ifeq ($(DEBUG),1)
	CFLAGS := $(CFLAGS) -g -DDEBUG=1 -Wall
endif

CLI_EXE := flexiname

CLI_OBJ := src/cli/main.o
LIB_OBJ := src/lib/queue.o src/lib/regex.o

all: options $(CLI_EXE)

options:
	@echo "Build options:"
	@echo " CFLAGS = $(CFLAGS)"
	@echo " CC     = $(CC)"
	@echo

$(CLI_EXE): $(CLI_OBJ) $(LIB_OBJ)
	$(LINK.c) -o $@ $(CLI_OBJ) $(LIB_OBJ)

install:
	install -Dm 755 $(CLI_EXE) $(DESTDIR)$(PREFIX)/bin/$(CLI_EXE)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(CLI_EXE)

clean:
	@-$(RM) $(CLI_EXE)
	@-$(RM) $(CLI_OBJ)
	@-$(RM) $(LIB_OBJ)
	@-$(RM) $(PROJECT)-$(VERSION).tar.gz

doc:
	doxygen

clean-doc:
	@-$(RM) -r doc

dist: clean
	@mkdir -p $(PROJECT)-$(VERSION)
	@cp -r AUTHORS  COPYING  ChangeLog  INSTALL  Makefile	README	src \
			$(PROJECT)-$(VERSION)
	@tar -cvzf $(PROJECT)-$(VERSION).tar.gz $(PROJECT)-$(VERSION)
	@rm -rf $(PROJECT)-$(VERSION)

.PHONY: all options clean dist install uninstall
