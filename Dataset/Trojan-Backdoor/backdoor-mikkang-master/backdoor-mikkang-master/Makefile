all: interpreter backdoor

interpreter:
	make -C $@

backdoor:
	make -C $@

clean:
	make -C interpreter clean
	make -C backdoor clean

.PHONY: clean interpreter backdoor
