#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <ctype.h>

#define h_addr h_addr_list[0]

void error(const char *msg)
{
perror(msg);
exit(0);
}

int main(int argc, char *argv[])
{
int clientfd, port_number, n;
struct sockaddr_in serv_addr;
char buffer[1024];
int user_input;
bool found = false;

if (argc != 3)
{
	fprintf(stderr, "usage %s hostname port\n", argv[0]);
	exit(0);
}

port_number = atoi(argv[2]);
memset(buffer, '0', sizeof(buffer));

clientfd = socket(AF_INET, SOCK_STREAM, 0);
if (clientfd < 0)
	error("ERROR socket NOT open.");

memset(&serv_addr, '0', sizeof(serv_addr));

serv_addr.sin_family = AF_INET;
port_number  = atoi(argv[2]);
serv_addr.sin_port = htons(port_number);

if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0){

	error("\n inet_pton error has occured!\n");

}

if (connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
{
	error("ERROR: Connection failed.\n");
}

printf("Connected to the server. Port: %d.\n", port_number);

printf("Server has selected a random number between 1-100.\n");

while (!found)
{	
	bzero(buffer, 1025);
	printf("Enter your guess:  ");
	char number_arr[3];
	scanf("%s", number_arr);
	bool is_valid_input = true;
	
	if (strlen(number_arr) > 3)
	{
		printf("Please select a number between 1-100\n");
		is_valid_input = false;
	}

	if (is_valid_input){
		
		send(clientfd, number_arr, strlen(number_arr), 0);
		bzero(buffer, 1024);
		read(clientfd, buffer, sizeof(buffer));
		printf("Server Reply with: %s\n", buffer);
		
		if(strcmp(buffer, "correct") == 0){
		found = true;
		}

		if (found)
			printf("You won!\n");
	}
	
}

return 0;
}

