
include ../config/Makefile.common

NB_MODULES = $(words $(MODULES))
ENTP_LIST  = $(subst /,_,$(MODULES:%=launch_%,))


build: $(OBJECTS)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/includes.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -D NB_MODULES="$(NB_MODULES)" \
			  	    -D ENTP_LIST="$(ENTP_LIST)"   \
				    -I "$(INCDIR)" -c $< -o $@


# TODO use absolute path for includes, so that modules can
# use each other public functions (maybe)
# XXX because of this .PHONY target, the whole core
# gets recompilated every time
.PHONY: $(INCDIR)/includes.h
$(INCDIR)/includes.h:
	@echo "$(MODULES)" | tr ' ' '\n' | \
	sed 's~\([^/]*\)/\([^/]*\)~#include "../../\1/\2/include/\2.h"~' \
	> $@

clean:
	rm -f $(OBJDIR)/*.o $(INCDIR)/includes.h


