1)Name : Sriragh Karat

2)Student ID : 555929009

3) The project essentially uses the concept of concept of socket programming to establish a client-server system that consists
of 3 different servers each containing the values of the key which is to be obtained by the clients from a text file. The server searches for the key
obtained from the corresponding client. If the key is not available in the server then it forwards it to server2 and after which it forwards it to server 3. We are not considering the boundary condition in which all three servers do not have the values for the given keys.

In this project the data is mainly is mainly stored and manipulated using an array of structures, pointers and arrays.

The project implements the text reading and writing using different string functions like strstr(),strcmp(),strncmp(),strcat() and strtok(). 

It also makes use of the file reading and writing functions such as fgets(), fputs(), sscanf(), fopen() and fclose().

Steps to implement the code

1. The required libraries are included.

2. In the client files there are is a user defined function for obtaining the key.

3. Create sockets, bind and use them for TCP.

4. Create sockets and use them for UDP.

5. The information is sent and received between the servers and the clients.

6. Required information is stored and printed.

7. Close the client sockets and server sockets after used.

8. The servers are in an infinite loop listening to the incoming information.

9. Client 1 and 2 are executed sequentially and not concurrently.


4) What the code files do: 

client1.c - It inputs a text which corresponds to the name of a university and the corresponding value is found. The value of the key is then
	    forwarded to server 1.

client2.c - Performs same function as client1

dhtserver1.c - The server 1 takes in the key from client1 and checks it against the corresponding values in the text file server1.txt. If the corresponding value of the given key is unavailable then the key is forwarded to server 2 else the value which is found is sent back to client 1
The value returned from server2 is stored in the cache.


dhtserver2.c - The server 2 takes in the key forwarded by server 1 and checks it against the corresponding values in the text file server2.txt
. If the 
corresponding value of the key is not found then key is forwarded to server 3 else the value is returned to server 1.
The value received from server 3 is stored in the cache.

dhtserver3.c - The server 3 takes the key forwarded by server 2 and checks it against the corresponding values in the text file server3.txt. The corresponding value of the key is returned to server 2.

5)Steps to run the file

1. run the make file which will compile all the C files

2. Once the files are compiled then run server 3, server2 and server1 in this order. 

3. The client program is run after this

4.The required university name is searched and the rest of the program is executed.


6)Issues regarding the code : No particular issue with the code and it works for all possible cases. The text would ideally require more spacing.

7)Reused code: The code from the following link was used as a model to develop this project:http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html 