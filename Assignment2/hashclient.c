#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Manifest constants */
#define MAX_BUFFER_SIZE 40


#define SERVER_IP "136.159.5.25"  /* csx1.cpsc.ucalgary.ca */
#define SERVER_PORT 12390
    
int main(void){
	struct sockaddr_in si_server;
	struct sockaddr *server;
	int s, i, len = sizeof(si_server);
	char buf[MAX_BUFFER_SIZE];
	int readBytes;

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		printf("Could not set up a socket!\n");
		return 1;
	}
      
	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(SERVER_PORT);
	server = (struct sockaddr *) &si_server;

	if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr)==0){
		printf("inet_pton() failed\n");
		return 1;
	}

	/* loop until the user enters "bye" */
	
	char hashNumber[MAX_BUFFER_SIZE];
	for( ; ; ){
		bzero(buf, MAX_BUFFER_SIZE);
		bzero(hashNumber, MAX_BUFFER_SIZE);

	 	printf("Enter your tweet (or \"bye\" to exit): \n");
		gets(buf);
	
		if(strncmp(buf, "bye", 3) == 0)
			break;
		
	

		if (sendto(s, buf, strlen(buf), 0, server, sizeof(si_server)) == -1){
			printf("sendto failed\n");
			return 1;
		}

		printf("Enter your desired hashing method:\n");
		printf(" 1 - Sequential index\n");
		printf(" 2 - Word sum\n");
		printf(" 3 - Your Hash\n");
		gets(hashNumber);

		if (sendto(s, hashNumber, strlen(hashNumber), 0, server, sizeof(si_server)) == -1){
			printf("sendto call failed");
			return 1;
		}

		

		if ((readBytes=recvfrom(s, buf, MAX_BUFFER_SIZE, 0, server, &len))==-1){
			printf("Read error!\n");
			return -1;
		}

		buf[readBytes] = '\0';    // proper null-termination of string
		if(strcmp(hashNumber, "1") == 0){
                        printf("You have chosen to use the sequential index hashing alg\n");
                } else if (strcmp(hashNumber, "2") == 0){
                        printf("You have chosen to use the word sum hasing alg\n");
                } else if (strcmp(hashNumber, "3") == 0){
                        printf("You have chosen to use the your hash alg\n");
                } else {
			printf("please enter the correct number");
			return 1;
		}

		printf("The hash is %s\n", buf);
	}
	close(s);
	return 0;
  }
