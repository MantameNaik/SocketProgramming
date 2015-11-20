FULL NAME: Shyam Shankar Mantame Naik
STUDENT ID: 6598929596

DESCRIPTION:
	There are 4 servers and a client. Each server sends its neighboring servers information to client. Each server sends information through TCP ports. Client collects all the information and creates a network topology using each node's information and the sends this network topology to each server via UDP ports.
The ports used are
	Server A = UDP 21596
	Server B = UDP 22596
	Server C = UDP 23596
	Server D = UDP 24596
	Client   = TCP 25596
		
The Client then uses Minimum Spanning Tree protocol to obtain the smallest cost of the network topology.

All the above mentioned details get printed on the terminal

SOURCE CODE:
	client.c : It first obtains the server info from each server through TCP socket. Calculate the matrix (network topology) and then sends it to each server via UDP port. Fianlly calculates the minimum cost using MST protocol.
	serverA.c : It reads the serverA.txt file for the neighbor's link cost information and sends them to client via TCP socket and then recieves the completed tree (network topology) from client using UDP socket.
	serverB.c : It reads the serverB.txt file for the neighbor's link cost information and sends them to client via TCP socket and then recieves the completed tree (network topology) from client using UDP socket.
	serverC.c : It reads the serverC.txt file for the neighbor's link cost information and sends them to client via TCP socket and then recieves the completed tree (network topology) from client using UDP socket.
	serverD.c : It reads the serverD.txt file for the neighbor's link cost information and sends them to client via TCP socket and then recieves the completed tree (network topology) from client using UDP socket.

RULES TO RUN THE CODE:
	1. Uncompress the ee450_mantamen_session3.tar.gz file
	2. Build: make all (make clean if required)
	3. Run: ./client.o
		./serverA.o
		./serverB.o
		./serverC.o
		./serverD.o
	(Please follow the same order and run each executable in a different window )
	4. The output will be printed in the run time during execution 

FORMAT OF MESSAGES:
	All messages are sent as arrays

IDIOSYNCRASY:
	It doesn't work properly if servers are booted up first before client.
	All codes use "localhost" for its IP address. 

REUSED CODE:
	Parts of TCP and UDP functions are used from beej's code
	Parts of MST's algorithm is used from Prim's code
	The used codes are modified according to the given spec and usage in commented in the source code


