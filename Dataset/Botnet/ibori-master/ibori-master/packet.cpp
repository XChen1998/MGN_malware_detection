#include "packet.h"
#include <netinet/in.h>
#include "libs/config.h"
#include "libs/libnet-macros.h"
#include "libs/libnet-headers.h"

#include <string>
#include <string.h>

// On/Off Switch For Dubugging
#define _PRINT_Debug 0
#define _PRINT_TCP_Segment 0
#define _PRINT_FILTER_PACKET_Debug 0

const uint MATCH_LEN = 3;               /* The length of matching characters */
const uint WORD_SIZE = MATCH_LEN+1;     /* "GET"/"POST" variable size */
const uint HTTP_REQUEST = 0;            /* Packet types */
const uint FTP = 1;
const uint TELNET = 2;

struct pcap_pkthdr header;


packet::packet(){}

packet::~packet(){}


bool packet::capturePacket(pcap_t **handle, char *dev, const char *filter_exp) {

    char errbuf[PCAP_ERRBUF_SIZE];          /* Error string */
    struct bpf_program fp;                  /* The compiled filter expression */
    bpf_u_int32 net;                        /* The IP of our sniffing device */

    *handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return false;
    }

    if (pcap_compile(*handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(*handle));
        return false;
    }

    if (pcap_setfilter(*handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(*handle));
        return false;
    }

    return true;
}


bool packet::getTCPsegment(char **tcp_segment, pcap_t *handle, timeval *ts) {

    /* ETHERTYPE_IP : 0x0800 */
    /* 14 : Ethernet MAC header size */
    /* 6 : tcp */
    /* 4 : Multiply by 4 to get byte count */

    const u_char *packet_t = pcap_next(handle, &header);
    if (packet_t == NULL) { printf("packet_t is NULL. \n"); return false; }
    else *ts = header.ts;

//    printf("1. header.ts.tv_sc : %ld\n", header.ts.tv_sec);
//    printf("2. ts.tv_sc : %ld\n", ts->tv_sec);

    libnet_ethernet_hdr* eth_hdr = (libnet_ethernet_hdr*) packet_t;
    if ( ntohs(eth_hdr->ether_type) != ETHERTYPE_IP )
        return false;

    libnet_ipv4_hdr* ip_hdr = (libnet_ipv4_hdr*) ((char*)eth_hdr + 14 );
    if ( ip_hdr->ip_p != 6 )
        return false;

    libnet_tcp_hdr* tcp_hdr = (libnet_tcp_hdr*) ((char*)ip_hdr + (ip_hdr->ip_hl * 4));

    u_int16_t tcp_seg_len = ntohs(ip_hdr->ip_len) - (u_int16_t)(ip_hdr->ip_hl * 4 + tcp_hdr->th_off * 4);

#if _PRINT_Debug
    /* Print Port number*/
    u_int16_t sport = ntohs(tcp_hdr->th_sport);
    u_int16_t dport = ntohs(tcp_hdr->th_dport);
    printf("%u->%u\n", sport, dport);

    /* Print its length */
    printf("Jacked packet length : [%d]", header.len);

    /* Print tcp segment's length */
    printf("\n IP Header Total length : [%d], Fragmented : [%d]", ntohs(ip_hdr->ip_len), ip_hdr->ip_off);
    printf("\n IP Header length : [%d]", ip_hdr->ip_hl*4);
    printf("\n TCP Header length : [%d]", tcp_hdr->th_off*4);
    printf("\n Tcp segment's length : [%d]\n", tcp_seg_len);
#endif

    if (tcp_seg_len <= 0) return false;    /* No TCP segments  */

    *tcp_segment = (char*)((char*)tcp_hdr + tcp_hdr->th_off * 4);

#if _PRINT_TCP_Segment
    /* Print TCP segment */
    printf("%s\n", *tcp_segment);
#endif

    return true;
}


bool packet::filterPacket(int *packet_type, char *tcp_segment, const uint PACKET_TYPE) {

    char str1[WORD_SIZE] = "";              /* "GET"/"POST" variable */
//    printf("**************tcp_seg : %s\n", tcp_segment);

    switch ( PACKET_TYPE ) {
        case HTTP_REQUEST :
            /* Filter Http request packets only. */
            strncpy( str1, tcp_segment, sizeof(char)*MATCH_LEN );
#if _PRINT_FILTER_PACKET_Debug
            printf("* str1 : %s\n", str1);
#endif
            if ( strcmp(str1, "GET") != 0 && strcmp(str1, "POS") != 0 ) {
#if _PRINT_FILTER_PACKET_Debug
                printf("GET/POST not matched!\n");
#endif
                return false;
            }
#if _PRINT_FILTER_PACKET_Debug
            printf("HTTP request(GET/POST) packet!\n");
#endif
            *packet_type = HTTP_REQUEST;
            return true;
            break;

        case FTP:

            *packet_type = FTP;
            return true;
            break;

        case TELNET:

            *packet_type = TELNET;
            return true;
            break;

        default :
        return false;
    }
}
