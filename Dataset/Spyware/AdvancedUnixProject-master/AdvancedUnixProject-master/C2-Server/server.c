#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

#include <asm/uaccess.h>
#include <asm/segment.h>

#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <linux/net.h>
#include <linux/ip.h>
#include <linux/in.h>

#define CONNECTED    1
#define DISCONNECTED 0
#define BUFFER_SIZE 1024

int connection_state;
char buffer[BUFFER_SIZE];
mm_segment_t old_fs;


struct sockaddr_in sock_in;
struct socket     *serv_sock;
struct socket     *acc_sock ;

void connect(void)
{
	// Instantiate a socket for communication.
	if (sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &serv_sock) < 0)
		printk("Could not create socket.\n");
	else
		printk("Socket created.\n");

	// Set properties for socket input.
	sock_in.sin_addr.s_addr = INADDR_ANY;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port   = htons((unsigned short) 0x539);

	// Bind socket with the properties denoted above.
	if (serv_sock->ops->bind(serv_sock, (struct sockaddr*)&sock_in, sizeof(sock_in)) < 0)
		printk("Error binding socket.\n");
	else
		printk("Socket bound.\n");

	// Listen on the socket for incoming connections.
	if (serv_sock->ops->listen(serv_sock, 32) != 0)
		printk("Error listening on socket.\n");
	else
		printk("Listening on socket.\n");

	// Create another socket for connection handling.
	if (sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &acc_sock) < 0)
		printk("Could not create socket.\n");
	else
		printk("Socket created.\n"); 

	// Clone the previously instantiated socket.
	acc_sock->type = serv_sock->type;
	acc_sock->ops  = serv_sock->ops;

	// Accept connections on the above IP:port
	if (acc_sock->ops->accept(serv_sock, acc_sock, 0) < 0)
		printk("Could not accept the socket connection.\n");
	else
	{
		printk("Socket connection accepted.\n");
		connection_state = CONNECTED;
	}
}

void recieve_message(struct socket *sock)
{
	ssize_t n;
	File *fp = fopen("got_file.txt", "a");
	if (fp)
	{
		// start writing from buffer
		while ((n = read(sock, buffer, BUFFER_SIZE)) > 0 )		
			fwrite(buffer, 1, n, fp);
		
		fclose(fp);
	}
	else
	{
		printk("cannot open file!\n");  
			fflush(stdout);
	}
	
}

int start(void)
{
	// Attempt to handle the TCP connection.
	connect();

	// Receive files
	while (connection_state)
		recieve_message(acc_sock);

	return 0;
}

void stop(void)
{
	if (serv_sock->ops->shutdown(serv_sock, 2) < 0)
		printk("Could not close the socket.\n");
	else
	{
		connection_state = DISCONNECTED;
		printk("Closed the socket.\n");
	}

	if (acc_sock->ops->shutdown(acc_sock, 2) < 0)
		printk("Could not close the socket.\n");
	else
	{
		connection_state = DISCONNECTED;
		printk("Closed the socket.\n");
	}
}

module_init(start);
module_exit(stop) ;

MODULE_LICENSE("GPL");
