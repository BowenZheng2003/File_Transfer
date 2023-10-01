// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 8080
#define MAXLINE 1024
	
int main() {
	//initialization
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in serverAddress, clientAddress;
		
	// Creating Socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	// //adds 0s to the end of server and client address
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
		
	// Filling server information
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);
		
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&serverAddress,
			sizeof(serverAddress)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	int len, n;
	len = sizeof(clientAddress); //len is value/result

	//receives message from client
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&clientAddress, &len); 
	buffer[n] = '\0';
	
	// checks if message starts with ftp and sends corresponding result
	if (strcmp(buffer, "ftp") == 0) {
    // If the client message is "ftp," reply with "yes"
    	const char *reply = "yes";
    	sendto(sockfd, reply, strlen(reply), MSG_CONFIRM, (const struct sockaddr *)&clientAddress, len);
	}else {
    // If the client message is not "ftp," reply with "no"
    	const char *reply = "no";
    	sendto(sockfd, reply, strlen(reply), MSG_CONFIRM, (const struct sockaddr *)&clientAddress, len);
	}
		
	return 0;
}

