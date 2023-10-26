#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
//Now define stuff
#define NAMELEN 15
#define BUFFERSIZE 1024
#define PORT 1337
// #define ADDR "192.168.1.855"

//(TODO: write code to clean client input)

// define client object
typedef struct {
	struct sockaddr_in address;
	int sfd;
	int uid;
	char name[15];
} client_t;
/*
int sti (char* string, int stringlen) {
	int arr[stringlen];
	for (int i = 0; i < stringlen; i++) {
		arr[i] = string[i];
	}
	return *arr;
}
*/
int main(int argc, char **argv) {
	printf("argc: %d\n", argc);	
	char ADDR[16] = {"\0"}; // 16 because it's the length of "192.168.xx.xxx" with some extra padding for safety
	if (argc > 1) {
		if (strlen(argv[1]) > 15) {
			perror("\nIllegal IP. Shutting down.\n");
			return 1;
		}
		for (int i = 0; i < strlen(argv[1]); i++) {
			ADDR[i] = argv[1][i];
		}
	} else {
		char presetaddr[15] = "192.168.1.855";
		for (int i = 0; i < 13; i++) {
			ADDR[i] = presetaddr[i];
		}
	}
	/*char ip[] = "192.168.0.085";
	  char buffer[2048] = {"\0"};
	  int listenfd = 0;
	  int connectfd = 0;*/
	//get zeroized suckers
	// make structs
	struct sockaddr_in server_info = {0};
	struct sockaddr_in client_info = {0};
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(1337);
	socklen_t server_info_len = sizeof(server_info);
	socklen_t client_info_len = sizeof(client_info);
	// connect/claim specific ip address
	if (0 > inet_pton(AF_INET, ADDR, &server_info.sin_addr)) {// &server_info.sin_family
	// inet_pton(AF_INET, inet_aton(ADDR, &server_info.sin_addr), &server_info.sin_addr)
		perror("ERROR IN IP.");
		return -1;
	}
	// make socket (below name is short for "socket file descriptor" or "socketfd")
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sfd) {
		perror("ERROR IN SOCKET.\n");
		printf("AF_INET: %d\nSOCK_STREAM: %d", AF_INET, SOCK_STREAM);
		return -1;
	}
	// checking bind
	if (0 > bind(sfd, (struct sockaddr*)&server_info, server_info_len)) {
		perror("ERROR IN BIND.\n");
		printf("sfd: %d\nserver_info_len: %u", sfd, server_info_len);
		return -1;
	}
	// checking listen
	if (0 > listen(sfd, 0)) {
		perror("WHY DIDN'T listen() WORK IT ONLY TAKES 1 ARG");
		printf("\nsfd: %d", sfd);
		return -1;
	}
	if (0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int*){1}, sizeof(int))) {
		perror("setsockopt() failed");
		return -1;
	}
	// checking accept
	if (0 > (accept(sfd, (struct sockaddr*)&client_info, &client_info_len))) {
		perror("ERROR IN ACCEPTING CONSEQUENCES\n");
		printf("sfd: %d\nserver_info_len: %u", sfd, server_info_len);
		return -1;
	}
	// now that we've got that out of the way, can't forget cfd
	int cfd = accept(sfd, (struct sockaddr*)&client_info, &client_info_len);
	// signals! thank goodness
	signal(SIGPIPE, SIG_IGN);
	// now we get to do cool stuff, woo!
	char *helloworld = "Hello, World!";
	ssize_t sendt = send(sfd, helloworld, strlen(helloworld), 0); // ssize_t sent =
	if (0 > sendt) {
		perror("ERROR IN SENDING DATA");
		return -1;
	}
	
	if (0 > close(cfd)) {
		perror("ERROR IN CLOSING CLIENT SOCKET");
		return -1;
	}
	if (0 > close(sfd)) {
		perror("ERROR IN CLOSING SFD");
		return -1;
	}
	return 0;
}
