
# User configurations

include config/Makefile.common

build: modules core

modules: $(MODULES)

.PHONY: $(MODULES) core

$(MODULES): 
	@echo '---> Making module "$@"'
	$(MAKE) build -C $@


core:
	@echo '---> Making "core"'
	$(MAKE) build -C $@ MODULES="$(MODULES)"

clean:
	for MOD in $(MODULES) core; do \
		$(MAKE) clean -C $$MOD ;   \
	done


