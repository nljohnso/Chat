//Niklaus Johnson
//CS457
//Project 1
//Using socket programming this program implements a chat client.

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#define PORT 0

struct packet {

	short version;
	short length;
	char payload[140];

};

int main(int argc, char *argv[]) {

	if(argc == 1) { //Server

		struct ifaddrs *ifap, *ifa;
		struct sockaddr_in *sa;
		char *addr;


		getifaddrs(&ifap);
		for(ifa = ifap; ifa; ifa = ifa->ifa_next) {
			if(ifa->ifa_addr->sa_family==AF_INET) { 
				sa = (struct sockaddr_in *) ifa->ifa_addr;
				addr = inet_ntoa(sa->sin_addr);
				if(strcmp(ifa->ifa_name, "eth0") == 0) {
					break;
				}
			}
		}
		freeifaddrs(ifap);
		
		printf("Welcome to Chat!\n");
		

		int socketfd, portNum, newSocket;
		socklen_t clientLen;

		char buffer[900];
		struct sockaddr_in serv_addr, cli_addr;
		int n;
		socketfd = socket(AF_INET, SOCK_STREAM, 0);
		if(socketfd < 0) {
			fprintf(stderr, "ERROR opening socket\n");
			exit(1);
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(PORT);
		
		if(bind(socketfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
			fprintf(stderr, "ERROR on binding\n");
			exit(1);
		}

		char hostname[128];

		socklen_t len = sizeof(serv_addr);
		if (getsockname(socketfd, (struct sockaddr *)&serv_addr, &len) == -1) {
    			fprintf(stderr, "ERROR on getsockname()\n");
		} else {
    			printf("Waiting for connection on %s port %d\n", addr, ntohs(serv_addr.sin_port));
		}
	
		listen(socketfd, 5);
		clientLen = sizeof(cli_addr);
		newSocket = accept(socketfd, (struct sockaddr *) &cli_addr, &clientLen);
		if(newSocket < 0) {
			fprintf(stderr, "ERROR on accept\n");
			exit(1);
		}
		printf("Found a friend! You receive first.\n");
		while(1){
			struct packet aPacket;
			bzero(aPacket.payload, 140);
			bzero(buffer, 900);
			n = read(newSocket, &aPacket, sizeof(aPacket));
			if(n < 0){
				fprintf(stderr, "ERROR reading from socket\n");
				exit(1);
			}

			printf("Friend: %s\n", aPacket.payload);
			

			while(1) {
				printf("You: ");
				bzero(buffer, 900);
				bzero(aPacket.payload, 140);
				fgets(buffer, 899, stdin);
				if(strlen(buffer) > 140) {
					printf("Error: Input too long.\n");
					continue;
				} else {
					break;
				}
			}

			aPacket.version = htons(457);
			aPacket.length = htons(strlen(buffer));
			strcpy(aPacket.payload, buffer);

			n = write(newSocket, &aPacket, sizeof(aPacket));
			if(n < 0){
				fprintf(stderr, "ERROR writing to socket\n");
				exit(1);
			}
		}


	} else if(argc > 2) { //Client

		int sockfd, portNum, n;
		struct sockaddr_in serv_addr;
		char* hostname;
		
		if(strcmp(argv[1], "-p") == 0) {
			portNum = atoi(argv[2]);
			hostname = argv[4];
		} else {
			portNum = atoi(argv[4]);
			hostname = argv[2];
		}
		
		char buffer[900];

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
			fprintf(stderr, "ERROR openingsocket\n");
			exit(0);
		}
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(hostname);
		serv_addr.sin_port = htons(portNum);
		if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
			fprintf(stderr, "ERROR connecting\n");
			exit(0);
		}

		printf("Connecting to server... Connected!\n");
		printf("Connected to a friend! You send first.\n");
		
		while(1) {
			struct packet aPacket;
			aPacket.version = htons(457);

			printf("You: ");
			bzero(buffer, 900);
			bzero(aPacket.payload, 140);
			fgets(buffer, 899, stdin);
			if(strlen(buffer) > 140) {
				printf("Error: Input too long.\n");
				continue;
			}

			aPacket.length = htons(strlen(buffer));
			strcpy(aPacket.payload, buffer);

			n = write(sockfd, &aPacket, sizeof(aPacket));
			if(n < 0){
				fprintf(stderr, "ERROR writing to socket");
				exit(0);
			}	
			
			bzero(aPacket.payload, 140);
			bzero(buffer, 140);
			n = read(sockfd, &aPacket, sizeof(aPacket));
			if(n < 0){ 
				fprintf(stderr, "ERROR reading from socket");
				exit(0);
			}	
	
			printf("Friend: %s", aPacket.payload);
		}
	} else {
		printf("Usage: \nStart server: \"./chat\"\nStart client: \"./chat -p PORT -s ADDRESS\"\n");
		exit(1);
	}
}
