/*
*	swap_client.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<stdbool.h>


#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256


extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);


int session_id = 0;

unsigned char S = '0';	// frame number sent
bool addBIT = false;
int count=0;

int swap_open(unsigned int addr, unsigned short port)
{
	int	sockfd;		// sockect descriptor
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;
    
	/*
 *	if the session is already open, then return error
 */
    
	if (session_id != 0)
		return -1;
    
	/*
 *	connect to a server
 */
    
	session_id = swap_connect(addr, port);	// in sdp.o
    
	/*
 *	return the seesion id
 */
    
	return session_id;
}


int swap_write(int sd, char *buf, int length)
{
	int wBytes;
    int rBytes;
	char frame[MAXFRAME];
    
	if (session_id == 0 || sd != session_id)
		return -1; // general error
    
    //Add control data
    //printf("Send window is currently: S = %c\n", S);
    //printf("Before adding Control: %s\n", buf);
    
    if (addBIT == false){
        memmove(buf+1, buf, length);
        buf[strlen(buf)+1] = '\0';
        printf("Adding control bit: %s\n", buf);
        addBIT = true;
    }
    
    buf[0] = S;
    
    strncpy(frame, buf, strlen(buf)+1); // Copy the buffer with added control bit
    frame[0] = S;
    
    //printf("After adding control bit:  %s\n",buf);
    
    //then we send that first frame
    wBytes= sdp_send(sd, buf, length); 
    
    //printf("SENT: %s\n\n", buf);
    while (wBytes == -1 ||  wBytes==-2)
    {// error
        
        printf("There was a  error sending the frame.Trying again\n");
        wBytes= sdp_send(sd, buf, length);//keep sending until it works xD
    }
    
    
    //printf("Send window is currently S = %c\n", S);
    // sdp_send returns an int representing: length (dropped), -1 (error), int num
    //of bytes written to the session 
    
    //We now wait for an ack frame. 
    //sdp_recieve_with_timer clears the buffer before reading into it so 0, then data.
    rBytes=sdp_receive_with_timer(sd,buf,5500);
    //check rBytes
    
    //Dissconnection case handle
    if (rBytes == -2){
        printf("Disconnection detected, Shutting down\n");
        swap_close(sd);
    }
    //timeout error
    if (rBytes == -3){
        printf("F:%d  NO ACK: %s\n", count, frame);//resend frame because we didnt get an ack
        wBytes=sdp_send(sd, frame,length);
        while (wBytes == -1 ||  wBytes==-2){
            // error
            
            printf("There was a  error sending the frame.Trying again\n");
            wBytes= sdp_send(sd, frame, length);//keep sending until it works xD
        }
        
        count++;
        printf("F:%d RESENT: %s  || S = %c || R = %c\n",count,frame,S,buf[0]);
        
        //Next S
        if(S =='0')
            S='1';
        else
            S='0';
        return wBytes;
    }
    
    //Next S
    char oldS = S;
    if(S =='0')
        S='1';
    else
        S='0';
    
    if (buf[0] == S ){ //The control frame received shuld match S
        printf("F:%d SENT: %s  || S = %c || R = %c\n",count,frame,oldS,buf[0]);
        count++;
        return wBytes; // go to the next frame
    }
    
    //Houston we have a problem
    else{
        //printf("Were landing in Texas!\n");
        printf("F:%d SENT: %s  || S = %c || R = %c\n",count++,frame,S,buf[0]);
        return wBytes;// Number of bytes writen
    }
}


void swap_close(int sd)
{
    if (session_id == 0 || sd != session_id)
        return;
    
    else
        session_id = 0;
    
    swap_disconnect(sd);	// in sdp.o
}
