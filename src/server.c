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

#include "http_parser/parser.h"

// wookie server data representation
struct wookie_server {
	int port;
	in_addr_t address;
	int listenfd;
};

// wookie client data representation
struct wookie_client {
	int connfd;
};

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	// get arguments
	struct wookie_client *client = (struct wookie_client*)arg;

	/* DEBUG */
	char *request = calloc(1024, sizeof(char*));
	ssize_t read_bytes = read(client->connfd, request, 1024 * sizeof(char*));

	printf("Size of char: %zd\n", sizeof(char));
	printf("Read %zd bytes out of max %zd bytes\n", read_bytes, 1024 * sizeof(char*));

	// request contains our HTTP request.
	struct parsed_result *result = malloc(sizeof(struct parsed_result*));
	result = parser_parse(request);

	// bye bye
	close(client->connfd);
	free(client);

	free(request);
	pthread_exit(0);
}

int wookie_start_server(char *host, int port) {
	// create server
	struct wookie_server *server = malloc(sizeof(struct wookie_server*));
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
		struct wookie_client *client = malloc(sizeof(struct wookie_client*));
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