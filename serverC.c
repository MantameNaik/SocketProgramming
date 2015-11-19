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
#define MYPORT "23596"	// the port users will be connecting to -- UDP
// get sockaddr, IPv4 or IPv6

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    	return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

in_port_t get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    	}
    return (((struct sockaddr_in6*)sa)->sin6_port);
}

int main() /*int argc, char *argv[])*/
{
    int sockfd = 0; 
    int sockfd2 = 0;
    //char buf[MAXDATASIZE] = {0};
    struct addrinfo hints, *servinfo, *p;
    int rv = 0;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN] = {0};
 
    int matrix[4][4];	

   FILE *f = NULL;
   char buf[100];
   char temp[100];
   int i =0;
   int j =0;
   int arr[4] = {0,0,0,0};
   char serv[100];
   char *prser[4];
   int count = 0;
   int cost = 0;
   char *ServIP;
/*---------------------Reading the file------------------*/
   	f = fopen("serverC.txt", "r");
	memset(buf,0,100);
	while(fgets(buf, sizeof(buf), f) != NULL) /* when fgets reaches EOF, it retuens NULL */
        {
                //printf("The line is %s\n",buf);
                memset(temp,0,100);
		count = 0;
                i = 0;
                while(buf[i] != '\n'){  // Reading each line
                        j=0;
                        while(buf[i]!=' ') // reading each string
                        {
			    if(buf[i] == '\0')
                                break;
			    if(buf[i] == '\n')
				break;
                            temp[j]=buf[i];
                            j++;
                            i++;
			}
                        temp[j] = '\0';
			//printf("temp value is %s\n", temp);
			if(count == 0){
			      strcpy(serv,temp);
			      //printf("serv value is %s\n", serv);
			      count++;
			}
			else{
			    if(strcmp(serv, "serverA")==0){
				cost = atoi(temp); 
				arr[0] = cost;
				prser[0] = serv;
			    }	
			    if(strcmp(serv, "serverB")==0){
                                cost = atoi(temp);
                                arr[1] = cost;
				prser[1] = serv;
                            }
			    if(strcmp(serv, "serverC")==0){
                                cost = atoi(temp);
                                arr[2] = cost;
				prser[2] = serv;
                            }
			    if(strcmp(serv, "serverD")==0){
                                cost = atoi(temp);
                                arr[3] = cost;
				prser[3] = serv;
                            }	
		       }
		i++;
		if(buf[i] == '\0' || buf[i] == '\n')
			break;
		}
	}
 	//printf("The array is %d,%d,%d,%d\n", arr[0],arr[1],arr[2],arr[3]);
	fclose(f);
/*-------------------------------------------------------------------------------*/
/*    if (argc != 2) {
 	fprintf(stderr,"usage: client hostname\n");
 	exit(1);
    }*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) {
 	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
 	return 1;
    }
 // loop through all the results and connect to the first we can
    
    for(p = servinfo; p != NULL; p = p->ai_next) {
 	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
 	     perror(" server: socket");
 	     continue;
	}

    	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
 	     close(sockfd);
 	     perror("Error: connect");
 	     continue;
    	}
 	break;
    }

    if (p == NULL) {
 	fprintf(stderr, "failed to connect\n");
 	return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    //printf("client: connecting to %s\n", s);
    printf("\nThe Server C is up and running.\n");
    freeaddrinfo(servinfo); // all done with this structure
    printf("The Server C has the following neighbor information:\n\nNeighbor------Cost\n");

    for(i=0;i<4;i++){
	if(arr[i]!= 0)
	    printf("%s       %d\n",prser[i],arr[i]);
    }       

    if (send(sockfd, &arr, sizeof(arr), 0) == -1){
        perror("send");
 	exit(1);
    }
    //ServIP = inet_ntoa((struct in_addr)p->ai_addr);

	printf("\nThe Server C finishes sending its neighbor information to\n" 
	"the Client with TCP port number %s and IP address %s\n",PORT,s); 
	struct sockaddr_in sa;
	int sa_len = sizeof(sa);
	getsockname(sockfd,(struct sockaddr * __restrict__)&sa,&sa_len);
	
	printf("\nFor this connection with the Client, the Server C has\n" 
	"TCP port number %d and IP address %s\n",((int)ntohs(sa.sin_port)),s);//(struct )(struct in_addr)sa->sin_addr);

/*-----------after sending info print here---------*/


    close(sockfd);

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

	//printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((recvfrom(sockfd2, &matrix, sizeof(matrix) , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	//printf("listener: got packet from %s\n",
	inet_ntop(their_addr.ss_family,	get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
/*	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);
*/
	printf("\nThe server C has received the network topology from the\n"
		"Client with UDP port number %d and IP address %s\n",(int)ntohs(get_in_port((struct sockaddr *)&their_addr)), s);

	printf("\nEdge------Cost\n");
	char row,col;
	for(i=0;i<4;i++){
          for(j=0;j<4;j++){
	     if(j >= i){
                 if(matrix[i][j] != 0){
		     switch(i){
			case 0: row = 'A';
			    break;
                        case 1: row = 'B';
                            break;
                        case 2: row = 'C';
                            break;
                        case 3: row = 'D';
                            break;	
			default: printf("error\n");
				 break;		
		     }
                     switch(j){
                        case 0: col = 'A';
                            break;
                        case 1: col = 'B';
                            break;
                        case 2: col = 'C';
                            break;
                        case 3: col = 'D';
                            break;
                        default: printf("error\n");                    
                                 break;
                     }	    
		     printf("%c%c         %d\n",row,col,matrix[i][j]);
		 }
 	     }		
          }
        }
	
	
	/*
 	struct sockaddr_in sa2;
        int sa2_len = sizeof(sa2);
        getsockname(sockfd2,(struct sockaddr * __restrict__)&sa2,&sa2_len);
	*/
	printf("\nFor this connection with Client, The Server C has\n"
	"UDP port number %s and IP address %s\n\n",MYPORT,s);//((int)ntohs(sa2.sin_port)),s);
	close(sockfd2);

    return 0;
}
