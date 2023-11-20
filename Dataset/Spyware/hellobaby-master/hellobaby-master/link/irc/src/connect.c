
/* Prevent windows.h from including winsock.h */
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN 
#endif

#include <windows.h>
#include <winsock2.h>
#include <inaddr.h>
#include <stdio.h>


/* TODO put this in the config module */
#define MAX_DOM_NAME 20
typedef struct {
    char name[MAX_DOM_NAME];
    int port;
} Server;

typedef struct {
    Server* servers;
    int length;
} ServList;


typedef struct {
    ServList sl;
} Config;
    

/**
 * Connect to the server `serv`. In case of failure try each 
 * IP associated to this server and return an error if none of
 * them works.
 * @param sock An IPv4 open socket
 * @param serv A struct of type Server which identifies the server
 *             to connect at
 * @return 0 in case of success, -1 otherwise
 */
int connectToServer(SOCKET sock, Server serv){

    // TODO change with getnameinfo 

    /* Resolve host name */
    struct hostent* hinfos = gethostbyname(serv.name);
    if (hinfos == NULL) { 
        fprintf(stderr,"Couldnt resolve: %s\n",serv.name);
        return -1;
    }

    
    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(serv.port);

    /* Try each IP of the list before failing */
    int i = 0; 
    while (hinfos->h_addr_list[i] != 0) {

        memcpy(&service.sin_addr.s_addr, 
               hinfos->h_addr_list[i++],
               hinfos->h_length);

        if (connect(sock, (SOCKADDR *) &service, sizeof (service)) == 0){
            // Successful connection
            printf("Connected to \"%s\"\n",inet_ntoa(service.sin_addr));
            return 0;
        } 

        fprintf(stderr,"Cannot connect to \"%s\"\n",
                inet_ntoa(service.sin_addr));
    }

    /* Every IP has failed */
    fprintf(stderr, "Failed attempt to connect to \"%s\"\n",hinfos->h_name);
    return -1;

}

int ircStart(SOCKET sock){
    char msg[] = "HELLO BABY ;)"; 
    send(sock, msg, sizeof msg, 0);
    
    return 0;
}





int serversWalk(ServList sl){

    /* Create IPv4 socket TODO add IPv6 socket*/    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) return -1;

    int i; printf("length: %d\n",sl.length);
    for (i = 0; i < sl.length; i++){
        if (connectToServer(sock, sl.servers[i]) == -1) continue;
        ircStart(sock);
        // TODO here --> disconnect from server
    }

    closesocket(sock);

    return 0;
}




/**
 * Start the Windows Socket API (trough a call to WSAStartup).
 * If it fails for an recoverable error, it will retry maxtries
 * times, sleeping one second before each try
 * @param maxtries Number of times WSAStartup must be called before
 *        returning an error.
 * @return 0 in case of success, -1 otherwise.
 */
int startWSA(int maxtries){

    WSADATA wsaData;
    int iResult;

    /* Try to start WSA maxtries times */
    while ( maxtries-- > 0 && 
           (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != NO_ERROR ){

        fprintf(stderr, "WSAStartup Failed. Remained: %d\n", maxtries);
        switch (iResult) {
            // Recoverable errors
            case WSASYSNOTREADY:
            case WSAEINPROGRESS:
            case WSAEPROCLIM: Sleep(1000);
                break;

            // Bad stuffs
            default: goto error;
        }

    }

    if (maxtries <= 0) goto error;
    return 0;

error: 
    fprintf(stderr, "WSAStartup error: %d\n", iResult);
    SetLastError(iResult); // XXX Don't know if this is gonna work
    return -1;
}




/**
 * This function will launch the network activities of the program.
 * @param config A structure of type Config obtained parsing the
 *               configuration file.
 * @return 0 in case of success, -1 otherwise. A call to WSAGetLastError
 *         will return the exact cause of the error.
 */
int startConnection(Config conf){

    /* Init Windows sockets API */
    if (startWSA(10) == -1)  return -1;

    /* Connect to one of the servers */
    if (serversWalk(conf.sl) == -1) goto error;

    /* Clean everything and return */
    if ( WSACleanup() != 0 ) goto error;
    return 0;

error:;
    /* Bkp error */
    int err = WSAGetLastError();
    WSACleanup();
    SetLastError(err);
    return -1;
}




int main(){

    /* XXX This will come from the config file */
    /* -------------- BEGIN ------------------*/
    Server irc_servers[2] = {
            { .name = "werew.tk", .port = 55555 },
            { .name = "moneystreet.tk", .port = 55555 }
    };

    ServList sl = { .servers = irc_servers, .length = 2 };

    Config conf = { .sl = sl };
    /* --------------- END -----------------*/



    /* Create socket */
    startConnection(conf);
    getchar();
    return 0;

}
