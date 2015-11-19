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
#include <limits.h>

#define PORT "25596" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once


// get sockaddr, IPv4 or IPv6:
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
 
// Number of vertices in the graph
#define V 4
#define false 0
#define true 1
// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int minKey(int key[], int  mstSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;
   int v = 0;
   for (v = 0; v < V; v++)
     if (mstSet[v] == false && key[v] < min)
         min = key[v], min_index = v;
 
   return min_index;
}
 
// A utility function to print the constructed MST stored in parent[]
int printMST(int parent[], int n, int graph[V][V])
{
   int i = 1;
   int cost = 0;
   char serv1, serv2;
   for (i = 1; i < V; i++){
	cost = cost + graph[i][parent[i]];
   }
   printf("\nThe Client has calculated a tree. The tree cost is %d\n",cost);
   printf("Edge------Cost\n");
   for (i = 1; i < V; i++){
      switch(i){
   	case 0: serv1 = 'A';
		break;
        case 1: serv1 = 'B';
                break;
        case 2: serv1 = 'C';
                break;
        case 3: serv1 = 'D';
                break;
      }
	switch(parent[i]){
        case 0: serv2 = 'A';
                break;
        case 1: serv2 = 'B';
                break;
        case 2: serv2 = 'C';
                break;
        case 3: serv2 = 'D';
                break;
      }
      printf("%c%c       %d \n", serv2, serv1, graph[i][parent[i]]);
   }
}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void primMST(int graph[V][V])
{
     int parent[V]; // Array to store constructed MST
     int key[V];   // Key values used to pick minimum weight edge in cut
     int mstSet[V];  // To represent set of vertices not yet included in MST
     int i = 0;
     int count = 0;
     int v = 0;
     // Initialize all keys as INFINITE
     for (i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;
 
     // Always include first 1st vertex in MST.
     key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
     parent[0] = -1; // First node is always root of MST 
 
     // The MST will have V vertices
     for (count = 0; count < V-1; count++)
     {
        // Pick thd minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, mstSet);
 
        // Add the picked vertex to the MST Set
        mstSet[u] = true;
 
        // Update key value and parent index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (v = 0; v < V; v++)
 
           // graph[u][v] is non zero only for adjacent vertices of m
           // mstSet[v] is false for vertices not yet included in MST
           // Update the key only if graph[u][v] is smaller than key[v]
          if (graph[u][v] && mstSet[v] == false && graph[u][v] <  key[v])
             parent[v]  = u, key[v] = graph[u][v];
     }
 
     // print the constructed MST
     printMST(parent, V, graph);
}

int main()
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    char ngbr[10];
    char ngbr2[10];
    int rv;
    int i,j,k;
    int numbytes;
    char buf[MAXDATASIZE];
    int temp_arr[4];
    int matrix[4][4];
    int UDPport;
    char MYPORT[20];
    char row,col;
    struct sockaddr_in sa2;
    int sa2_len = sizeof(sa2);

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
      printf("\nThe Client has TCP port number %s and IP address %s\n", PORT,"127.0.0.1");//p->ai_addr);//get_in_addr((struct sockaddr *)&their_addr))
//    printf("server: waiting for connections...\n");
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
  //  printf("server: got connection from %s\n", s);
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

    //printf("client: received %d, %d, %d, %d\n",temp_arr[0],temp_arr[1],temp_arr[2],temp_arr[3]);
        close(new_fd); // parent doesn't need this
        switch(i){
		case 0: strcpy(ngbr,"ServerA");
			break;
        	case 1: strcpy(ngbr,"ServerB");
                	break;
        	case 2: strcpy(ngbr,"ServerC");
                	break;
        	case 3: strcpy(ngbr,"ServerD");
                	break;
        	default: printf("error\n");//strcpy(ngbr,"ServerA");
                	break;
    	}
    	printf("\nThe Client receivers neighbor information from the %s\n"
	   "TCP port number %d and IP address %s\n",ngbr,(int)ntohs(get_in_port((struct sockaddr *)&their_addr)),s);
	printf("The %s has the following neighbor information:\nNeighbor------Cost\n",ngbr);
	for(j=0;j<4;j++){
            if(temp_arr[j]!= 0){
		switch(j){
                case 0: strcpy(ngbr2,"ServerA");
                        break;
                case 1: strcpy(ngbr2,"ServerB");
                        break;
                case 2: strcpy(ngbr2,"ServerC");
                        break;
                case 3: strcpy(ngbr2,"ServerD");
                        break;
                default: printf("error\n");//strcpy(ngbr,"ServerA");
                        break;
                }
            	printf("%s       %d\n",ngbr2,temp_arr[j]);
	    }
        }
	
	printf("For this connection with %s,The Client has \nTCP port number %s and IP address %s\n",ngbr,PORT,s); 


    }// end of for loop for each server
/* MATRIX here
	i = 0; j= 0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }*/
    close(sockfd);

/*-----------------------------------UDP-------------------------------*/
	i = j = 0;	
    for(i=0;i<4;i++){
        sleep(1);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	UDPport = 21596 + i*(1000);
	sprintf(MYPORT,"%d",UDPport);
        //printf("The UDP port is %s\n",MYPORT);
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
	   if ((numbytes = sendto(sockfd, &matrix, sizeof(matrix), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	   }

	switch(i){
                case 0: strcpy(ngbr,"ServerA");
                        break;
                case 1: strcpy(ngbr,"ServerB");
                        break;
                case 2: strcpy(ngbr,"ServerC");
                        break;
                case 3: strcpy(ngbr,"ServerD");
                        break;
                default: printf("error\n");//strcpy(ngbr,"ServerA");
                        break;
        }

	printf("The Client has sent the network topology to the %s\n"
	"with UDP port number %s and IP address %s as follows:\nEdge------Cost\n",ngbr,MYPORT,s);
	freeaddrinfo(servinfo);

	for(k=0;k<4;k++){
          for(j=0;j<4;j++){
             if(j >= k){
                 if(matrix[k][j] != 0){
                     switch(k){
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
                     printf("%c%c         %d\n",row,col,matrix[k][j]);
                 }
             }
          }
        }
/*
 	struct sockaddr_in sa2;
        int sa2_len = sizeof(sa2);*/
        getsockname(sockfd,(struct sockaddr * __restrict__)&sa2,&sa2_len);

	printf("For this connection with %s, The Client has\n" 
	"UDP port number %d and IP address %s\n",ngbr,((int)ntohs(sa2.sin_port)),s);
	//printf("talker: sent %d bytes to %s\n", numbytes, "localhost");
	close(sockfd);
    }
/*-------------------------------*/
    primMST(matrix);//[4][4]);	
    return 0;
}
