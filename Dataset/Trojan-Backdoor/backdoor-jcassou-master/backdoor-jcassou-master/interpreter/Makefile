SOURCES = minivm.h minivm.c
CFLAGS += -Wall -Wextra
BUILDDIR = build

all: $(BUILDDIR) $(BUILDDIR)/libminivm.a $(BUILDDIR)/interpreter

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/libminivm.a: $(BUILDDIR)/minivm.o
	ar rcs $(BUILDDIR)/libminivm.a $(BUILDDIR)/minivm.o

$(BUILDDIR)/minivm.o: minivm.c
	$(CC) $(CFLAGS) -g -c minivm.c -o $@

$(BUILDDIR)/interpreter: interpreter.c $(BUILDDIR)/libminivm.a
	$(CC) $(CFLAGS) -g $^ -o $@

clean:
	rm -rf $(BUILDDIR)

inspect: bytecode.txt
	od --format=u1 --width=4 bytecode.txt

sloc:
	@$(foreach srcfile, $(SOURCES), printf "%-10s : %s\n" `cat $(srcfile) | grep . | wc -l` $(srcfile);)
