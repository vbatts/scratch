
PRGNAM := lolcat
CC := gcc
CFLAGS := -O2 -g 

bindir := /usr/bin

all: $(PRGNAM)

$(PRGNAM): $(PRGNAM).c
	$(CC) $(CFLAGS) -DHAVE_STATE -o $@ $<

no-state: $(PRGNAM).c
	$(CC) $(CFLAGS) -o $(PRGNAM) $<

install: $(PRGNAM)
	install -D -m 0755 -s $(PRGNAM) $(DESTDIR)$(bindir)/$(PRGNAM)

clean:
	rm -rf $(PRGNAM) *.o *~

