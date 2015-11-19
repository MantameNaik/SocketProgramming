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

#define PORT "25596" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once

//UDP
//#define UDPPORTA "4950"// the port users will be connecting to

//

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    	return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int i,j;
    int numbytes;
    char buf[MAXDATASIZE];
    int temp_arr[4];
    int matrix[4][4];
    int UDPport;
    char MYPORT[20];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
 	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
    }

 // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
 	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	    perror("server: socket");
            continue;
 	}
 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
 	    sizeof(int)) == -1) {
 	    perror("setsockopt");
 	    exit(1);
 	}

	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
 	    close(sockfd);
 	    perror("server: bind");
 	    continue;
 	}

 	break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
 	fprintf(stderr, "server: failed to bind\n");
 	exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
 	perror("listen");
 	exit(1);
    }

    //sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
 
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
 	perror("sigaction");
 	exit(1);
    }

    printf("server: waiting for connections...\n");
//use for loop for 4 servers
  //  while(1) { // main accept() loop
    for(i=0;i<4;i++){ // This loop reads from each server.
	sin_size = sizeof their_addr;
 	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
 	if (new_fd == -1) {
 	    perror("accept");
 	    continue;
 	}
	
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);
/*
    if (!fork()) { // this is the child process
 	close(sockfd); // child doesn't need the listener
 	if (send(new_fd, "Hello, world!", 13, 0) == -1)
 	    perror("send");
        close(new_fd);
        exit(0);
    }
*/
	// arr is 4x4
	// temp_arr is 1x4
    if ((recv(new_fd, temp_arr, sizeof(temp_arr), 0)) == -1) {
        perror("recv");
        exit(1);
    }
    
	matrix[i][0] = temp_arr[0];
	matrix[i][1] = temp_arr[1];
  	matrix[i][2] = temp_arr[2];
	matrix[i][3] = temp_arr[3];

    printf("client: received %d, %d, %d, %d\n",temp_arr[0],temp_arr[1],temp_arr[2],temp_arr[3]);
    close(new_fd); // parent doesn't need this
    }
    for(i=0;i<4;i++){
	for(j=0;j<4;j++){
	    printf("%d ",matrix[i][j]);
	}
	printf("\n");
    }
    close(sockfd);
/*-----------------------------------UDP-------------------------------*/
    for(i=0;i<4;i++){
        sleep(1);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	UDPport = 21596 + i*(1000);
	sprintf(MYPORT,"%d",UDPport);
        printf("The UDP port is %s\n",MYPORT);
	if ((rv = getaddrinfo("localhost", MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}
//	for(i=0;i<4;i++){
	   if ((numbytes = sendto(sockfd, &matrix, sizeof(matrix), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	   }
//	}

	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", numbytes, "localhost");
	close(sockfd);
    }
/*-------------------------------*/
    return 0;
}
