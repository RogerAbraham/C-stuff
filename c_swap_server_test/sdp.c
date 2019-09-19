/*
*	misc.c
*
*		swap_connect() and swap_accept() should not used together
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <arpa/inet.h>

#define	MAXLINE	256	// maximum characters to receive and send at once


int sockfd;
unsigned int opponent_addr; //set to either the server or client depending
unsigned short opponent_port; //"                                        "
int sssn_id = 0;


int swap_connect(unsigned int addr, unsigned short port)
{
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;
    
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("swap_connect: can not open a socket\n");
		return -1;
	}
    
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = addr;
	servaddr.sin_port = port;
    
	buf[1] = 0xff;
	n = sendto(sockfd, buf, 10, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
	if (n < 0)
		return -1; //error
	else {
		opponent_addr = addr; // set the global server and port num to the passed ones
        opponent_port = port;
		sssn_id++;
        uint32_t ip = 2110443574;
        
        struct in_addr ip_addr;
        ip_addr.s_addr = opponent_addr;
        printf("\nsdpSwap_Connect:  addr=%s, port=%d\n", inet_ntoa(ip_addr),(unsigned int)(ntohs(opponent_port)));
		return sssn_id;
	}
}


int swap_disconnect(int sd)
{
	if (sd != sssn_id)
		return -1;
    
	char buf[MAXLINE];
    
	buf[1] = 0xfe;	// disconnect
    
	sdp_send(sd, buf, strlen(buf)+1);
}


int swap_accept(unsigned short port)
{
	fd_set rfds;
	struct timeval tv;
	int retval;
    //sockfd defined as int globally
	struct	sockaddr_in	servaddr;	// server address
	struct	sockaddr_in	cliaddr;	// server address
	char	buf[MAXLINE];//Its own 128 byte buffer
    //char    buf2[MAXLINE]; //copy of the buffer
	int	len, n,test;
    
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("swap_accept: can not open a socket\n");
		return -1;
	}
    
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = port;
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		printf("swap_accept: can not bind\n");
		return -1;
	}
    
	while(1) {
		while(1) {
			FD_ZERO(&rfds);
			FD_SET(sockfd, &rfds);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
			if (retval)
				break;
		}
        
		len = sizeof(cliaddr);
        n=recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len); 
        
        //printf("accept: received, n = %d, %c\n", n, (unsigned char)buf[0]);
        if ((unsigned char)buf[1] == 0xff) {
            //printf("accept: received connection request\n");
			break;
        }
    }
    //sets the global addr and ports to Senders address and ports
    opponent_addr = cliaddr.sin_addr.s_addr;
    opponent_port = cliaddr.sin_port;
    //printf("swap_accept (Server): addr=%s, port=%s\n", ntohl(opponent_addr), ntohs(opponent_port));
    //printf("swap_accept(Client): addr=%s, port=%s\n", ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));
    sssn_id++;
    
    return sssn_id;
}


int sdp_receive(int sd, char *buf)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    
    struct	sockaddr_in	cliaddr;
    int	len, n;
    
    if (sd != sssn_id)
        return -1;	// general error
    
    while(1) {
        while(1) {
			FD_ZERO(&rfds);
			FD_SET(sockfd, &rfds);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
			if (retval)
                break;
        }
        
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        
        if ((unsigned char)buf[1] == 0xfe)  // disconnection from the opponent SWAP
			return -2;  // disconnection
        //	It is done in sdp_send().
        //		if (random() % 100 < 20 && n > 0)
        //			continue;  // let's drop it
        if (random() % 100 < 50 && n > 10) {
            printf("T");
			buf[n-1] = 0xff;
        }
        
        break;
    }
    
    if (n <= 0)
        return -1;	// general error
    else if ((unsigned char)buf[1] == 0xfe) 
        return -2;	// disconnected
    else
        return n;
}


// expiration in milliseconds
int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    
    struct	sockaddr_in	cliaddr;
    int	len, n;
    
    if (sd != sssn_id)
        return -1;	// general error
    
    while(1) {
        
        /*
        *	set timer
        */
        
        while(1) {
			FD_ZERO(&rfds);
			FD_SET(sockfd, &rfds);
			tv.tv_sec = expiration / 1000;
			tv.tv_usec = (expiration % 1000) * 1000;	// usec -> microsecond, not millisecond
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
			if (retval)
                break;
			else
                return -3;	// timeout error
        }
        
        /*
        *	receive data
        */
        
        
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        
        /*
        *	if disconnected
        */
        if (n > 0 && (unsigned char)buf[1] == 0xfe)
        {printf("-2  cuz N > 0 and buf[1] == 0xfe");
            return -2;}
        /*
        *	include some errors
        */
        if (random() % 100 < 20)
        {//printf("TiMeOuT");
            return -3;}  // timeout error
        //This just garbles a bit
        if (random() % 100 < 20 && n > 10) 
        {printf("The Garbler Garbled a BiT :o");
            buf[n-1] = 0xff;}
        
        break;
    }
    
    if (n <= 0)
    {return -1;}	// general error
    else if ((unsigned char)buf[1] == 0xfe)
    {printf("-2 cuz buf[1] == 0xfe o.O");
        return -2;}	// disconnected
    else
    {return n;}
}


int sdp_send(int sd, char *buf, int length)
{
    struct	sockaddr_in	cliaddr;
    int	len, n;
    
    if (sd != sssn_id)
        return -1;
    
    //10% of packets are dropped
    //What happens here is we do nothing to the buffer and just return
    if (random() % 100 < 10)
        return -2;  // let's drop it
    
    // the last 80% of packets
    
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = opponent_addr;
    cliaddr.sin_port = opponent_port;
    
    struct in_addr ip_addr2;
    ip_addr2.s_addr = opponent_addr;
    
    //printf("Sending: data=%s  addr=%s, port=%d\n",buf,inet_ntoa(ip_addr2),(unsigned int)(ntohs(opponent_port)));
    
    n = sendto(sockfd, buf, length, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    //printf("sdp_send: after sending\n");
    
    return n;
}


