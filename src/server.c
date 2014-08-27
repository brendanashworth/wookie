// server.c

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// struct that is a wookie connected client
typedef struct {
	int listenfd;
	int connfd;
} wookie_client;

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	// get arguments
	wookie_client *client = (wookie_client*)arg;
	int listenfd = client->listenfd;
	int connfd = client->connfd;

	// hold entire request
	char *request;

	// set buffer and clear it
	char *buffer = malloc(1025);
	memset(&buffer, '\0', sizeof(buffer));

	// read until you get \n\n
	while (1) {
		// read more bytes
		size_t read_bytes = read(connfd, &buffer, sizeof(buffer));
		(&buffer)[1024] = '\0'; // null terminate

		printf("read %zu, buffer: %s\n", read_bytes, buffer);

		// append the buffered request to the request
		request = malloc(strlen(request) + strlen(buffer) + 1);
		strcat(request, buffer);

		// reset buffer
		memset(&buffer, '\0', sizeof(buffer));

		if (request[sizeof(request) - 1] == '\n' && request[sizeof(request) - 2] == '\n') {
			printf("finished http request!");
			break;
		} else {
			printf("End of buffer is: %c and %c", request[strlen(request)], request[strlen(request) - 1]);
		}
	}

	free(buffer);

	// buffer contains our HTTP request.

	// bye bye
	close(connfd);
	free(request);
	pthread_exit(0);
}

int wookie_start_server(char *host, int port) {
	int listenfd = -1;
	struct sockaddr_in serv_addr;

	// open the socket
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// set some memory
	memset(&serv_addr, '0', sizeof(serv_addr));

	// set server address and stuff
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(inet_network(host));
	serv_addr.sin_port = htons(port);

	// bind to ip
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	// backlog of 10
	listen(listenfd, 10);

	// check for errors
	if (listenfd == -1) {
		printf("ERROR: Could not bind to socket %s:%d", host, port);
		return 1;
	}

	printf("Now listening.\n");

	// get clients
	while (1) {
		// accept client
		int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		// make wookie_client and pass it to a new thread
		wookie_client *client;
		client->listenfd = listenfd;
		client->connfd = connfd;

		pthread_t thread;
		pthread_create(&thread, NULL, wookie_handle_client, client);
		pthread_detach(thread);

		// sleep
		sleep(1);
	}

}