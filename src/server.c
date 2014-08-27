// server.c

// comment this line out to disable multithreading
#define MULTITHREADING

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

// wookie server data representation
typedef struct {
	int port;
	in_addr_t address;
	int listenfd;
} wookie_server;

// wookie client data representation
typedef struct {
	int connfd;
} wookie_client;

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	// get arguments
	wookie_client *client = (wookie_client*)arg;

	// hold entire request
	char *request = "";

	// set buffer and clear it
	char *buffer = malloc(1025);
	memset(&buffer, '\0', sizeof(buffer));

	// read until you get \n\n
	while (1) {
		// read more bytes
		size_t read_bytes = read(client->connfd, &buffer, sizeof(buffer));
		//-> buffer[1024] = '\0'; // this needs to be null terminated!

		// append the buffered request to the request
		printf("Allocating: %lu, read_bytes: %zu, strlen(buffer): \n", strlen(request) + read_bytes + 1, read_bytes, strlen(buffer));
		request = malloc(strlen(request) + read_bytes + 1);

		strcat(request, buffer); // now dies here
		printf("but not here.\n");

		// reset buffer
		memset(&buffer, '\0', sizeof(buffer));

		// check for end of request
		if (request[sizeof(request) - 1] == '\n' && request[sizeof(request) - 2] == '\n') {
			printf("finished http request!");
			break;
		} else {
			printf("End of buffer is: %c and %c", request[strlen(request)], request[strlen(request) - 1]);
		}
	}

	// free the buffer
	free(buffer);

	// request contains our HTTP request.

	// bye bye
	close(client->connfd);
	free(client);

	free(request);
	pthread_exit(0);
}

int wookie_start_server(char *host, int port) {
	// create server
	wookie_server *server = malloc(sizeof(wookie_server*));
	server->port = port;
	server->address = inet_network(host);
	server->listenfd = -1;

	// open the socket
	int on = 1;
	server->listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(server->listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	// setup the server address
	struct sockaddr_in server_address;
	memset(&server_address, '0', sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(server->address);
	server_address.sin_port = htons(server->port);

	// bind to ip
	bind(server->listenfd, (struct sockaddr*)&server_address, sizeof(server_address));

	// backlog of 10
	int result = listen(server->listenfd, 10);

	// check for errors
	if (result == -1) {
		printf("ERROR: Could not bind to socket %s:%d\n", host, server->port);
		free(server);
		return 1;
	}

	printf("Now listening.\n");

	// get clients
	while (1) {
		// make wookie_client and pass it to desired handler; wookie_handle_client free's the memory
		wookie_client *client = malloc(sizeof(wookie_client*));
		client->connfd = accept(server->listenfd, (struct sockaddr*)NULL, NULL);

		#ifdef MULTITHREADING
		// multithreaded handling of clients
		pthread_t thread;
		pthread_create(&thread, NULL, wookie_handle_client, client);
		pthread_detach(thread);
		#else
		// this handles it in the main thread, which can be a massive issue to performance.
		wookie_handle_client(client);
		#endif

		// sleep
		sleep(1);
	}

	// close server
	close(server->listenfd);
	free(server);
}