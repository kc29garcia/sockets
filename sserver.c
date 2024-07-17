/*Run sserver to recieve port number
 * connect clients to play the game by running: ncat server port#
 * giver client a name 
 * then start making guesses*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

void pexit(char *errmsg) {
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

#define MAXUSERS 100
#define MAXLEN 100
char userids[MAXUSERS][MAXLEN];
int clientfds[MAXUSERS];
int numUsers=0;

//introduce a function for thread -- work as dedicated server for a client
void *dedicatedServer(void *ptr) {
	int clientId = (int) ptr;
}

int get_rand_num(){

	srand(time(NULL));
	int rand_sel = (rand() % (100 - 1)) + 1;
	return rand_sel;

}

int isLower(int input, int  randomNumber){
	return input < randomNumber;
}

int isHigher(int input, int  randomNumber){
	return input > randomNumber;
}

int isEqual(int input, int  randomNumber){
	return input == randomNumber;
}


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char buffer[1025];
    time_t ticks; 

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		pexit("socket() error.");
		
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int port = 4999;
	do {
		port++;
    	serv_addr.sin_port = htons(port); 
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
	printf("bind() succeeds for port #%d\n", port);

    if (listen(listenfd, 10) < 0)
		pexit("listen() error.");
	
	int randomNumber = get_rand_num();

    while(1)
    {	
		//numUsers++;
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		printf("connected to client %d.\n", numUsers);

		//read the userid here & store userid & connfd in global arrays
		read(connfd, buffer, sizeof(buffer));
		char *cptr = strchr(buffer, '\n');
		if (cptr)
			*cptr = '\0';

		strcpy(userids[numUsers], buffer);
		clientfds[numUsers] = connfd;

		//broadcast to all other users that a new user has joined?
		sprintf(buffer, "%s joined.\n", userids[numUsers]);
	
		//send this to all previous clients!
		for(int i=0; i<numUsers; i++)
			write(clientfds[i], buffer, strlen(buffer));

		// create a  thread here for dedicated server with current numUsers as parameter?
		numUsers++;
		
		
		int n;
		bzero(buffer, 1025);
		
		while((n = read(connfd, buffer, sizeof(buffer))) > 0){
		
			int input = atoi(buffer);
			bzero(buffer, 1025);

			if(isLower(input, randomNumber)){
		
				strcpy(buffer, "Higher ");
		
			}	

			else if(isHigher(input, randomNumber)){
		
				strcpy(buffer, "Lower ");
		
			}

			else{
		
				strcpy(buffer, "Correct!");
		
			}
		
			send(connfd, buffer, strlen(buffer), 0);

			}

       		close(connfd);
		
	   		exit(0);
}
}
