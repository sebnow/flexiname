DEBUG := 1
VERSION := "git"

CFLAGS := -O2 -pipe -DVERSION=$(VERSION)
ifeq ($(DEBUG),1)
	CFLAGS := $(CFLAGS) -g -DDEBUG=1
endif

CLI_OBJ := src/cli/main.o
LIB_OBJ := src/lib/queue.o

all: options cli

options:
	@echo "Build options:"
	@echo " CFLAGS = $(CFLAGS)"
	@echo " CC     = $(CC)"
	@echo

cli: $(CLI_OBJ) $(LIB_OBJ)
	$(LINK.c) -o $@ $(CLI_OBJ) $(LIB_OBJ)

install:
	install -m 755 cli $(DESTDIR)$(PREFIX)/bin/flexiname

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/flexiname

clean:
	@-$(RM) $(CLI_OBJ) $(LIB_OBJ)
