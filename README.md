Chat
====

Simple chat program in C (using TCP socket connections)

Author: Niklaus Johnson

Class: CS457


To start the Server of chat:

	(1) Before envoking any programs be sure to run the Makefile.  This will create an executable by the name, "chat."

	(2) To start the server run the executable "chat" in the terminal with no other arguments.  This will start up the server with the local ip address and a random, open, port.

To start the Client of chat:

	(1) The server needs to be running before attempting to have a client connect to chat.
	
	(2) To start a client run the executable "chat" with the arguments that inclue a port and a hostname. An example of this is: "./chat -p 3790 -s 192.168.47.232" Where the -p flag indicates the port to connect to and the -s flag indicates the IP address of the server. Order does not matter.

For more help:

	(1) For additional help (usage statements) run the chat executable with the "-h" flag shown here: "./chat -h"
