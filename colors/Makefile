
PRGNAM := colors
CC := gcc
CFLAGS := -O2 -g 

bindir := /usr/bin

all: $(PRGNAM)

$(PRGNAM): $(PRGNAM).c
	$(CC) $(CFLAGS) -DHAVE_STATE -o $@ $<

install: $(PRGNAM)
	install -D -m 0755 -s $(PRGNAM) $(DESTDIR)$(bindir)/$(PRGNAM)

clean:
	rm -rf $(PRGNAM) *.o *~

