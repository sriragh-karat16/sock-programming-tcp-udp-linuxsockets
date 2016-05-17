superobj: dhtserver3 dhtserver2 dhtserver1 client1 client2

dhtserver3: dhtserver3.o
	gcc -o dhtserver3 -g dhtserver3.o -lsocket -lnsl -lresolv

dhtserver2: dhtserver2.o
	gcc -o dhtserver2 -g dhtserver2.o -lsocket -lnsl -lresolv

dhtserver1: dhtserver1.o
	gcc -o dhtserver1 -g dhtserver1.o -lsocket -lnsl -lresolv

client1: client1.o
	gcc -o client1 -g client1.o -lsocket -lnsl -lresolv

client2: client2.o
	gcc -o client2 -g client2.o -lsocket -lnsl -lresolv

dhtserver3.o: dhtserver3.c
	gcc -g -c dhtserver3.c

dhtserver2.o: dhtserver2.c
	gcc -g -c dhtserver2.c

dhtserver1.o: dhtserver1.c
	gcc -g -c dhtserver1.c

client1.o: client1.c
	gcc -g -c client1.c

client2.o: client2.c
	gcc -g -c client2.c

clean:
	rm -f *.o dhtserver3 dhtserver2 dhtserver1 client1 client2
