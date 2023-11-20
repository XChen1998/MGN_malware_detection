### FIXME

all:
	make -C interpreter 
	make -C backdoor
	ocaml str.cma  compiler/compiler.ml test/test.mini test/testbyte
	ocaml str.cma  compiler/compiler.ml login/login.mini login/loginbyte

interpreter:
	make -C $@

backdoor:
	make -C $@

.PHONY: clean
clean:
	make -C interpreter clean
	make -C backdoor clean	
	rm test/testbyte
	rm login/loginbyte