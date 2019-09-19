/*
*	swap_server.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define	MAXLINE	128	// maximum characters to receive and send at once
#define MAXFRAME 256


extern int swap_accept(unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);


int session_id = 0;
int count = 0;
unsigned char R = '0';	// frame number to receive


int swap_wait(unsigned short port)
{
	/*
 *	if the session is already open, then return error
 */
    
	if (session_id != 0)
		return -1;
    
	/*
 *	accept a connection
 */
    
	session_id = swap_accept(port);	// in sdp.o
    
	/*
 *	return a ssession id
 */
    
	return session_id;
}


int swap_read(int sd, char *buf)
{
	int	rBytes;
    int    wBytes;
	char	ack[MAXFRAME];
    
	if (session_id == 0 || sd != session_id)
		return -1;
    
	/*
 *	receive a frame without a timer
 */
    
    rBytes= sdp_receive(sd,buf);
    
    while(buf[0] != R){
        //resend ack.
        ack[0] = R;
        ack[1] = '\0';
        printf("ERR: %d  REC: %s || badACK = %c || RESENDING ACK: %c\n", count++, buf,buf[0], ack[0]);
        //count++;
        wBytes = sdp_send(sd, ack, 1);//send ACK again
        while(wBytes == -1 || wBytes == -2 ){//incase we drop the ack frame
            printf("ACK frame dropped, Resending\n");
            wBytes = sdp_send(sd, ack, 1);
        }
        
        rBytes= sdp_receive(sd,buf);
    }
    //contains sender frame buf
	// check the first bit, buff[0] if it matchs our current R then send a ACK frame back which is just a buffer with a single value of R + 1.
    
    if (buf[0] == R) { //R=S which is good so we increment R and send ACK[R]
        
        //next R
        if(R =='0')
            R='1';
        else
            R='0';
        
        ack[0] = R;
        ack[1] = '\0';
        
        //printf("Current Receive frame: %c\n", R);
        printf("F: %d  REC: %s || ACK = %c\n", count, buf,ack[0]);
        //printf("Preparing to send ACK: %s\n",ack);
        wBytes = sdp_send(sd, ack, 1);//send ACK
        count++;
        while(wBytes == -1 || wBytes == -2 ){//incase we drop the ack frame
            printf("ACK frame dropped, Resending\n");
            wBytes = sdp_send(sd, ack, 1);
        } 
        
        //printf("ACK sent\n\n");
        return rBytes;
    }
    
    //if (buf[0] != R ) { //If we recieve an out of order frame:
    
    //} 
    
    if (rBytes==-2){
        printf("Client Disconnected from server\n");
        swap_close(sd);
    }
    
    if (rBytes==-1){
        printf("There was a general error\n");
    }
    
    exit(0);
}


void swap_close(int sd)
{
    if (session_id == 0 || sd != session_id)
        return;
    
    else
        session_id = 0;
    
    swap_disconnect(sd);	// in sdp.o
}
