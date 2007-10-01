PROJECT := "flexiname"
VERSION := "git"
DEBUG := 1

CFLAGS := -ansi -O2 -pipe -DVERSION=$(VERSION)
ifeq ($(DEBUG),1)
	CFLAGS := $(CFLAGS) -g -DDEBUG=1 -Wall
endif

CLI_EXE := flexiname

CLI_OBJ := src/cli/main.o
LIB_OBJ := src/lib/queue.o src/lib/regex.o src/lib/rename.o

all: options $(CLI_EXE)

options:
	@echo "Build options:"
	@echo " CFLAGS = $(CFLAGS)"
	@echo " CC     = $(CC)"
	@echo

$(CLI_EXE): $(CLI_OBJ) $(LIB_OBJ)
	@echo "CC  -o $@"
	@$(LINK.c) -o $@ $(CLI_OBJ) $(LIB_OBJ)
	@echo

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

doc-clean:
	@-$(RM) -r doc

dist: clean
	@mkdir -p $(PROJECT)-$(VERSION)
	@git log | git shortlog > ChangeLog
	@cp -r AUTHORS  COPYING  ChangeLog  INSTALL  Makefile	README	src \
			$(PROJECT)-$(VERSION)
	@echo "Compressing archive"
	@tar -cvzf $(PROJECT)-$(VERSION).tar.gz $(PROJECT)-$(VERSION) > /dev/null
	@rm -rf $(PROJECT)-$(VERSION)

dist-clean:
	@rm -rf $(PROJECT)-$(VERSION)

%.o: %.c
	@echo "CC  $<"
	@$(COMPILE.c) -o $*.o $<

%.d: %.c
	@echo "DEP $<"
	@$(CC) -o $*.d -MM -E $< > /dev/null

.PHONY: all options clean dist install uninstall doc-clean
