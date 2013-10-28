/*
 * USAGE: ./server <port> <filename> <sleeptime>
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	/* open() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>	/* usleep(), read() */
#include <stdlib.h>	/* exit() */
#include <string.h>	/* strncpy() */

# define MSS	1448

int main (int argc, char **argv)
{
	int sockfd, newsockfd, filefd, portno, sleeplen;
	unsigned int clilen;
	char filename[256], buffer[1500];
	ssize_t ret;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 4) {
		printf("FATAL: missing parameters\n");
		exit(1);
	}
	portno = atoi(argv[1]);
	strncpy(filename, argv[2], 256);
	sleeplen = atoi(argv[3]);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		 sizeof(serv_addr)) < 0) {
		perror("bind()");
		exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0) {
		perror("accept()");
		exit(1);
	}

	/* Open file */
	if ((filefd = open(filename, O_RDONLY)) == -1) {
		perror("open()");
		exit(1);
	}

	/* Read from file and send via socket */
	while ((ret = read(filefd, buffer, MSS)) != 0) {
		int flag = 1;
		if (ret == -1) {
			perror("read()");
			close(filefd);
			exit(1);
		}
		if (setsockopt(newsockfd, IPPROTO_TCP, 1, (char *) &flag, sizeof(int)) < 0) {
			perror("setsockopt()");
			close(filefd);
			exit(1);
		}
		if (write(newsockfd, buffer, ret) < 0) {
			perror("write() socket");
			close(filefd);
			exit(1);
		}
		/* DAMA control loop */
		usleep(sleeplen);
	}

	close(filefd);
	return 0;
}
