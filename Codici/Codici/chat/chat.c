/* Simple IPv4 UDP bidirectional chat */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char**argv)
{ 
    int sockfd, n;
    extern int errno;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in );
    char mesg[1000];
   
    if(argc < 4)
    { 
        printf("Use: Destination_IP Destination_Port Listening_PORT");
	    return 0;
    }	 

   
    
   if (!fork())
   {    // receiver
        if((sockfd=socket(PF_INET,SOCK_DGRAM,0)) <0)
        { 
            printf("\nError in socket opening ... exit!");
            return -1;
        }


        memset(&local_addr,0,sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_port=htons(atoi(argv[3]));
        if( bind(sockfd, (struct sockaddr *) &local_addr, len)<0)
        { 
            printf("\nBinding error! Error code n.%d \n",errno);
            return -1;
        }

        for (;;)
        { 
            n = recvfrom(sockfd,mesg,999,0,(struct sockaddr *) &remote_addr,&len);
            mesg[n] = 0;
	        printf("From IP:%s Port:%d msg:%s \n", inet_ntoa(remote_addr.sin_addr),  ntohs(remote_addr.sin_port), mesg);
        }
     
        return 0;
   }

   else
   {   
        if((sockfd=socket(PF_INET,SOCK_DGRAM,0)) <0)
        { 
            printf("\nError in socket opening ... exit!");
            return -1;
        }
  
        memset(&remote_addr,0,len);
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr)); 
        remote_addr.sin_port=htons(atoi(argv[2]));
   
        while (fgets(mesg, 1000,stdin) != NULL)
        { 
            sendto(sockfd,mesg,strlen(mesg),  0, (struct sockaddr *)&remote_addr, len);
        }
        return 0;
   }
         
}
