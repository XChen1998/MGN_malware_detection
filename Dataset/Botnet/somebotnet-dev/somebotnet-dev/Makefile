# Makefile

CFLAGS=-ggdb
LINUX_CC=/usr/bin/gcc
LINUX_VERS=some_bin_test.1

__linux__ : source/some_main.c source/some_utils.c source/some_serv.c
	${LINUX_CC} ${CFLAGS} -o ${LINUX_VERS} source/some_main.c source/some_utils.c source/some_serv.c source/some_cmd.c

clean :
	rm -fr ${LINUX_VERS} src/*.o *.o
