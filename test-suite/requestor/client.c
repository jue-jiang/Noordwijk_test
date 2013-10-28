#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	/* open() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>	/* read() */
#include <stdlib.h>	/* exit() */
#include <string.h>	/* strncpy() */
#include <netdb.h>

int main (int argc, char **argv)
{
	int sockfd, filefd, portno;
	char filename[256], buffer[1500];
	ssize_t ret;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 4) {
		printf("FATAL: missing parameters\n");
		exit(1);
	}
	portno = atoi(argv[1]);
	strncpy(filename, argv[2], 256);
	server = gethostbyname(argv[3]);

	if (server == NULL) {
		perror("gethostbyname()");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
	      server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect()");
		exit(1);
	}

	/* Open file */
	if ((filefd = open(filename, O_CREAT | O_WRONLY, 0777)) == -1) {
		perror("open()");
		exit(1);
	}

	/* Read from socket and save to file */
	while ((ret = read(sockfd, buffer, 1500)) != 0) {
		if (ret == -1) {
			perror("read() socket");
			close(filefd);
			exit(1);
		}
		setsockopt(sockfd, IPPROTO_TCP, 12, (int[]){1}, sizeof(int));
		if (write(filefd, buffer, ret) < 0) {
			perror("write()");
			close(filefd);
			exit(1);
		}
	}

	close(filefd);
	return 0;
}
