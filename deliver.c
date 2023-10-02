// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char* argv[]) {
	//initialization
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in serverAddress;
	char *input = NULL;
	int port = atoi(argv[2]);
	char* servAddress = argv[1];

	//if wrong amount of arguments, exits
	if (argc != 3){
		return 1;
	}

	// allocate memory to store user input
    input = (char *)malloc(256);

	//get user input
    printf("Please Enter A File In The Following Format (ftp <file name>): ");
    fgets(input, MAXLINE, stdin);

	if (strncmp(input, "ftp ", 4) != 0){
		printf("Must start with a ftp\n");
		exit(1);
	}

	//removes 'ftp' part of input
	char *file_to_send = input + 4;

	// Remove any trailing whitespace or newline characters
	int length = strlen(file_to_send);

	while (length > 0 && (file_to_send[length - 1] == ' ' || file_to_send[length - 1] == '\n')) {
    	file_to_send[length - 1] = '\0';
    	length--;
	}

	//checks if the file doesn't exist	
    if (access(file_to_send, F_OK) != 0){
		printf("File Doesn't Exist\n");
        exit(1);
    }

	// Creating Socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("Socket Files");
		exit(EXIT_FAILURE);
	}
	
	//adds 0s to the end of server address
	memset(&serverAddress, 0, sizeof(serverAddress));
		
	// Filling server information
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = inet_addr(servAddress);
		
	int n, len;
		
	//sends a message to the server
	sendto(sockfd, "ftp", strlen("ftp"), MSG_CONFIRM, (const struct sockaddr *) &serverAddress, sizeof(serverAddress));
	printf("Ftp message sent to server.\n");
			
	//receives the message from the server
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &serverAddress, &len);
	
	//end message sent from server
	buffer[n] = '\0';

	//compares server answer and gives corresponding message
    if (strcmp(buffer, "yes") == 0){
        printf("A file transfer can start.\n");
    } else {
        printf("Exiting");
    }
	
	//frees the memory
    free(input);
	//closes the socket (terminates connection)
	close(sockfd);
	return 0;
}
