
include ../../config/Makefile.common

CFLAGS += -I $(INCLUDES)


build: $(MODOUT)

-include $(DEPS)

$(MODOUT): $(SOURCES)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SOURCES) -o $@

clean:
	rm -f $(MODOUT)



