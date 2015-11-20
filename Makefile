all: client serverA serverB serverC serverD

client: client.c
	gcc -g client.c -o client.o -lsocket -lnsl -lresolv

serverA: serverA.c
	gcc -g serverA.c -o serverA.o -lsocket -lnsl -lresolv

serverB: serverB.c
	gcc -g serverB.c -o serverB.o -lsocket -lnsl -lresolv

serverC: serverC.c
	gcc -g serverC.c -o serverC.o -lsocket -lnsl -lresolv

serverD: serverD.c
	gcc -g serverD.c -o serverD.o -lsocket -lnsl -lresolv

clean: 
	-rm -f client serverA serverB serverC serverD
