#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char**argv) {
	// addr
	char ADDR[16] = {"\0"};
	if (argc > 1) {
		if (strlen(argv[1]) > 15 || strlen(argv[1]) < 15) {
			perror("\n Screw You\n");
			return -1;
		}
		for (int i = 0; i < (int) strlen(argv[1]); i++) {
			ADDR[i] = argv[1][i];
		}
	} else {
		char tmp[14] = "192.168.1.855";
		for (int i = 0; i < 15; i++) {
			ADDR[i] = tmp[i];
		}
	}
	// create info
	struct sockaddr_in server_info = {0};
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = AF_INET;
	server_info.sin_port = htons(1337);
	socklen_t server_info_len = sizeof(server_info);
	// join ip
	if (0 > inet_pton(AF_INET, ADDR, &server_info.sin_addr)) {
		perror("Error in making connection");
		return -1;
	}
	// create socket
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sfd) {
		perror("socket error");
		return -1;
	}

	// connect to server with socket
	if (0 > connect(sfd, (struct sockaddr*)&server_info, server_info_len)) {
		perror("connection error");
		return -1;
	}
	char buffer[1024] = {"\0"};
	// do cool (and maybe client) stuff
	ssize_t recvd = recv(sfd, &buffer, 1024, 0);
	if (0 > recvd) {
		perror("ERROR IN RECIEVE");
		return -1;
	}
	printf("%s\n", buffer);
	
	// clean up
	if (0 > close(sfd)) {
		perror("ERROR IN close(sfd)");
		return -1;
	}
	return 0;
}
