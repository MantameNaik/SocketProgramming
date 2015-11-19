/********************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/* TCP Client for sending its data*/
#define PORT "25596" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define MYPORT "24596"   // the port users will be connecting to -- UDP
// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    	return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main()//int argc, char *argv[])
{
    int sockfd = 0; 
    int sockfd2 = 0;
    char buf[MAXDATASIZE] = {0};
    struct addrinfo hints, *servinfo, *p;
    int rv = 0;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN] = {0};
    FILE *fd = NULL; 
    int arr[4] = {0,10,0,0}; 
    int matrix[4][4];
    int i,j;
    /*int nbLinkcost[3][2] = { {1,20}, {2,10} };*/ 
/*	
    if (argc != 2) {
 	fprintf(stderr,"usage: client hostname\n");
 	exit(1);
    }
*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) {
 	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
 	return 1;
    }
 // loop through all the results and connect to the first we can
/*
    fd = fopen("serverA.txt", "r");
    if(fd == NULL){
	fprintf(stderr,"Error: Can't open file\n");
        exit(1);
    }
    fclose(fd);*/
// to work here
	
    
//  till here
    for(p = servinfo; p != NULL; p = p->ai_next) {
 	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
 	     perror(" Could not create ServerA - socket");
 	     continue;
	}

    	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
 	     close(sockfd);
 	     perror("Error: ServerA: connect");
 	     continue;
    	}
 	break;
    }

    if (p == NULL) {
 	fprintf(stderr, "client: failed to connect\n");
 	return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure

    if (send(sockfd, &arr, sizeof(arr), 0) == -1){
        perror("send");
 	exit(1);
    }

    close(sockfd);

/*-------------------- part a------------*/
/*-------------------- UDP recieve------------*/
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
        }

        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd2 = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
                        perror("listener: socket");
                        continue;
                }

                if (bind(sockfd2, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd2);
                        perror("listener: bind");
                        continue;
                }

                break;
        }

        if (p == NULL) {
                fprintf(stderr, "listener: failed to bind socket\n");
                return 2;
        }
        freeaddrinfo(servinfo);

        printf("listener: waiting to recvfrom...\n");

        addr_len = sizeof their_addr;
        if ((recvfrom(sockfd2, &matrix, sizeof(matrix) , 0,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(1);
        }

        printf("listener: got packet from %s\n",
                inet_ntop(their_addr.ss_family,
                        get_in_addr((struct sockaddr *)&their_addr),
                        s, sizeof s));
/*      printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buf);
*/
        for(i=0;i<4;i++){
          for(j=0;j<4;j++){
            printf("%d ",matrix[i][j]);
          }
          printf("\n");
        }

        close(sockfd2);


    return 0;
}
