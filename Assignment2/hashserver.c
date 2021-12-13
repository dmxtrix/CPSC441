#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Manifest constants */
#define MAX_BUFFER_SIZE 40
#define PORT 12390

int main(){

	struct sockaddr_in si_server, si_client;
	struct sockaddr *server, *client;
	int s, i, len=sizeof(si_server);
	char messagein[MAX_BUFFER_SIZE];
	char messageout[MAX_BUFFER_SIZE];
	int readBytes;

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		printf("Could not setup a socket!\n");
		return 1;
	}
    
	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	server = (struct sockaddr *) &si_server;
	client = (struct sockaddr *) &si_client;

	if (bind(s, server, sizeof(si_server))==-1){
		printf("Could not bind to port %d!\n", PORT);
		return 1;
	}
	printf("wordlenserver now listening on port %d...\n", PORT);
	
	char hashin[MAX_BUFFER_SIZE];
	int hash;
	/* big loop, looking for incoming messages from clients */
	for( ; ; ){
		/* clear out message buffers to be safe */
		bzero(messagein, MAX_BUFFER_SIZE);
		bzero(messageout, MAX_BUFFER_SIZE);

		//get tweet from the client
		if ((readBytes=recvfrom(s, messagein, MAX_BUFFER_SIZE, 0, client, &len)) < 0){
	    		printf("Read error!\n");
	    		return -1;
	  	}

		//get which hashing method from the client
		if ((readBytes=recvfrom(s, hashin, MAX_BUFFER_SIZE, 0, client, &len)) < 0){
			printf("recvfrom() call failed");
			return -1;
		}

		hash = atoi(hashin);

		if(hash == 1){
			printf("You have chosen to use the sequential index hashing alg\n");			
			//do sequential hashing
			strcpy(messageout, messagein);
		} else if (hash == 2){
			//do word sum hashing
			printf("You have chosen to use the word sum hashing alg\n");
			strcpy(messageout, messagein);
		} else if (hash == 3){
			//do your hash hashing 
			printf("You have chosen to use the your hash alg\n");
			strcpy(messageout, messagein);
		}


		
		sendto(s, messageout, strlen(messageout), 0, client, len);		
	      }

    close(s);
    return 0;
}
