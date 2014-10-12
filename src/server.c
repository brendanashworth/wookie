// server.c

// multithreading options (comment multithreading out to go synchronously)
#define EWOK_WORKERS 10

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

#include "http-parser/http_parser.h"

#include "memory.h"
#include "cluster.h"
#include "util.h"

// wookie server data representation
struct wookie_server {
	int port;
	in_addr_t address;
	int listenfd;
	wookie_framework *framework;
};

// wookie client data representation
struct wookie_client {
	int connfd;
	wookie_server *server;
};

// Callback for the HTTP parser, simply forwards to wookie_framework_request
int on_message_complete(http_parser *parser, const char *at, size_t length) {
	printf("Swag %s\n", at);

	// Pass the request to the framework
	wookie_framework_request(parser);


	return 0;
}

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	DEBUG("wookie_handle_client received call");

	http_parser_settings settings;
	settings.on_message_complete = on_message_complete;

	http_parser *parser = w_malloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = arg;

	// get arguments
	wookie_client *client = (wookie_client*)arg;

	DEBUG("http_parser was initiated");

	size_t nparsed, len = 80 * 1024;
	char buf[len];
	ssize_t recved;

	DEBUG("Initiated some variables regarding buffering and parsing");

	recved = recv(client->connfd, buf, len, 0);
	if (recved < 0) {
		// Error
		close(client->connfd);
	}
	DEBUG("Recieved some bytes from the client and put into the buffer");

	nparsed = http_parser_execute(parser, &settings, buf, recved);
	DEBUG("Executed the HTTP parser");

	// did we get the same amount of bytes?
	if (recved != nparsed) {
		// Error
		close(client->connfd);
	}

	// the http parser will then call on_message_complete() with the appropriate data
	DEBUG("wookie_handle_client is returning out");
	return NULL;
}

int wookie_start_server(wookie_framework *framework, char *host, int port) {
	DEBUG("wookie_start_server called");

	// create server
	wookie_server *server = w_malloc(sizeof *server);
	server->port = port;
	server->address = inet_network(host);
	server->listenfd = -1;
	server->framework = framework;

	// open the socket
	int on = 1;
	server->listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server->listenfd == -1) {
		printf("ERROR: Could not create socket.");

		close(server->listenfd);
		w_free(server);
		return 1;
	}
	DEBUG("Created socket");

	// set the socket options
	setsockopt(server->listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	// setup the server address
	struct sockaddr_in server_address;
	memset(&server_address, '\0', sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(server->address);
	server_address.sin_port = htons(server->port);

	// bind to ip
	int result = bind(server->listenfd, (struct sockaddr*)&server_address, sizeof(server_address));
	if (result == -1) {
		printf("ERROR: %s: Could not bind to socket %s:%d\n", strerror(errno), host, server->port);

		close(server->listenfd);
		w_free(server);
		return 1;
	}
	DEBUG("Bound to socket");

	// listen with a backlog of 10
	result = listen(server->listenfd, 10);
	if (result == -1) {
		printf("ERROR: %s Could not listen on socket %s:%d\n", strerror(errno), host, server->port);

		close(server->listenfd);
		w_free(server);
		return 1;
	}
	DEBUG("Started listening");

	printf("Now listening...\n");

	// get clients
	while (1) {

		// make wookie_client and pass it to desired handler; wookie_handle_client free's the memory
		wookie_client *client = w_malloc(sizeof *client);
		client->connfd = accept(server->listenfd, (struct sockaddr*)NULL, NULL);
		client->server = server;

		DEBUG("Accepted new client");

		// multithreaded handling of clients
		// TODO: don't spawn a new thread with the client waiting :(
		pthread_t thread;
		pthread_create(&thread, NULL, wookie_handle_client, client);
		pthread_detach(thread);
		DEBUG("Spawned and detached client thread to wookie_handle_client");
	}

	// close server
	close(server->listenfd);
	w_free(server);
}
