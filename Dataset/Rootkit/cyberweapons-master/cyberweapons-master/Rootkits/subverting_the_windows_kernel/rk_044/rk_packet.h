
#ifndef __PACKET_H__
#define __PACKET_H__

/*
 * defines and such
 */
#define ETHERNET_HEADER_LENGTH   14
#define RESERVED(_p) ((PPACKET_RESERVED)((_p)->ProtocolReserved))
#define TRANSMIT_PACKETS    128
#define MAX_POSITIONS 32

/* types
 *
 */
typedef struct _BFRF {
	PVOID mBuf;
	ULONG mLen;
} BFRF, *PBFRF;

///////////////////////////////////////////////////////////////////////////
// packet structures
///////////////////////////////////////////////////////////////////////////
typedef long n_long;
typedef short n_short;
typedef long n_time;
typedef unsigned short u_short;
typedef unsigned long u_long;
typedef unsigned char u_char;

#define ETH_ALEN		6		/* Octets in one ethernet addr	 */
#define ETH_P_ARP		0x0806	/* Address Resolution packet	*/
#define ARPHRD_ETHER	0x01
#define ARPOP_REQUEST	0x01
#define ARPOP_REPLY		0x02

#define ETH_P_IP		0x0800	/* Internet Protocol packet	*/
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_ARP		0x0806		/* Address Resolution packet	*/

/*
 * Protocols
 */
#define IPPROTO_IP              0               /* dummy for IP */
#define IPPROTO_ICMP            1               /* control message protocol */
#define IPPROTO_IGMP            2               /* group management protocol */
#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
#define IPPROTO_TCP             6               /* tcp */
#define IPPROTO_PUP             12              /* pup */
#define IPPROTO_UDP             17              /* user datagram protocol */
#define IPPROTO_IDP             22              /* xns idp */
#define IPPROTO_ND              77              /* UNOFFICIAL net disk proto */

#define IPPROTO_RAW             255             /* raw IP packet */
#define IPPROTO_MAX             256


struct   in_addr {
    union   {
         struct{
             unsigned  char   s_b1,
                              s_b2,
                              s_b3,
                              s_b4;
        }  S_un_b;
             struct  {
             unsigned  short  s_w1,
                              s_w2;
              }  S_un_w;
               unsigned long  S_addr;
     } S_un;
};

struct sockaddr {
    unsigned short sa_family;
    char           sa_data[14];
};



// Ethernet Header
struct ether_header 
{
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	unsigned short	h_proto;		/* packet type ID field	*/
};

struct arphdr
{
	unsigned short	ar_hrd;		/* format of hardware address	*/
	unsigned short	ar_pro;		/* format of protocol address	*/
	unsigned char	ar_hln;		/* length of hardware address	*/
	unsigned char	ar_pln;		/* length of protocol address	*/
	unsigned short	ar_op;		/* ARP opcode (command)		*/
};

struct	ether_arp {
	struct	arphdr ea_hdr;	/* fixed-size header */
	u_char	arp_sha[ETH_ALEN];	/* sender hardware address */
	u_char	arp_spa[4];	/* sender protocol address */
	u_char	arp_tha[ETH_ALEN];	/* target hardware address */
	u_char	arp_tpa[4];	/* target protocol address */
};
#define	arp_hrd	ea_hdr.ar_hrd
#define	arp_pro	ea_hdr.ar_pro
#define	arp_hln	ea_hdr.ar_hln
#define	arp_pln	ea_hdr.ar_pln
#define	arp_op	ea_hdr.ar_op

/* IP Header in Little Endian */
struct iphdr {
	u_char	ip_hl:4,		/* header length */
			ip_v:4;			/* version */
	u_char	ip_tos;			/* type of service */
	short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	short	ip_off;			/* fragment offset field */
#define	IP_DF 0x4000		/* dont fragment flag */
#define	IP_MF 0x2000		/* more fragments flag */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
};

/////////////////////////////////////////////////////////////////////////
typedef	u_long	tcp_seq;

// TCP header. Per RFC 793, September, 1981. In Little Endian
struct tcphdr {
	u_short	th_sport;		/* source port */
	u_short	th_dport;		/* destination port */
	tcp_seq	th_seq;			/* sequence number */
	tcp_seq	th_ack;			/* acknowledgement number */
	u_char	th_x2:4,		/* (unused) */
		    th_off:4;		/* data offset */
	u_char	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	u_short	th_win;			/* window */
	u_short	th_sum;			/* checksum */
	u_short	th_urp;			/* urgent pointer */
};

struct pseudo_header
{
  struct in_addr source_address;
  struct in_addr dest_address;
  unsigned char placeholder;
  unsigned char protocol;
  unsigned short tcp_length;
};

struct pps
{
	struct ether_header eh;
	struct ether_arp ea;
};

/*
 * Prototypes
 */
VOID OnOpenAdapterDone	( IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS Status, IN NDIS_STATUS OpenErrorStatus );
VOID OnCloseAdapterDone	( IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS Status );
VOID OnSendDone		( IN NDIS_HANDLE ProtocolBindingContext, IN PNDIS_PACKET pPacket, IN NDIS_STATUS Status );
VOID OnTransferDataDone	( IN NDIS_HANDLE ProtocolBindingContext, IN PNDIS_PACKET Packet, IN NDIS_STATUS Status, IN UINT BytesTransferred );
VOID OnResetDone	( IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS Status );
VOID OnRequestDone	( IN NDIS_HANDLE ProtocolBindingContext, IN PNDIS_REQUEST pRequest, IN NDIS_STATUS Status );
NDIS_STATUS OnReceiveStub      ( IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_HANDLE MacReceiveContext, IN PVOID HeaderBuffer, IN UINT HeaderBufferSize, IN PVOID LookAheadBuffer, IN UINT LookaheadBufferSize, IN UINT PacketSize );
VOID OnReceiveDoneStub	( IN NDIS_HANDLE ProtocolBindingContext );
VOID OnStatus		( IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS Status, IN PVOID StatusBuffer, IN UINT StatusBufferSize );
VOID OnStatusDone	( IN NDIS_HANDLE ProtocolBindingContext );
NTSTATUS OnReadStub( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp );

/* use this to send raw packets */
VOID SendRaw (char *c, int len);
void ReturnDataToClient(char *theClientData, int theClientLen);

extern BFRF GlobalPtrArray[MAX_POSITIONS]; /* max recv packets waiting */
extern ULONG GPFront;


#endif
