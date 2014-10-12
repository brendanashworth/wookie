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

#include "../deps/http-parser/http_parser.h"

#include "memory.h"
//#include "parser.h"
#include "cluster.h"

// wookie server data representation
struct wookie_server {
	int port;
	in_addr_t address;
	int listenfd;
	wookie_framework *framework;
	http_parser_settings *settings;
};

// wookie client data representation
struct wookie_client {
	int connfd;
	wookie_server *server;
};

// Callback for the HTTP parser, simply forwards to wookie_framework_request
static int on_message_complete(http_parser *parser) {
	// Pass the request to the framework
	wookie_framework_request(parser);

	return 0;
}

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	// get arguments
	wookie_client *client = (wookie_client*)arg;

	http_parser parser;
	http_parser_init(&parser, HTTP_REQUEST);
	parser.data = client->connfd;

	size_t nparsed, len = 80 * 1024;
	char buf[len];
	ssize_t recved;

	recved = recv(client->connfd, buf, len, 0);
	if (recved < 0) {
		// Error
		close(client->connfd);
	}

	nparsed = http_parser_execute(&parser, client->server->settings, buf, recved);

	// did we get the same amount of bytes?
	if (recved != nparsed) {
		// Error
		close(client->connfd);
	}

	// the http parser will then call on_message_complete() with the appropriate data
	return NULL;
}

int wookie_start_server(wookie_framework *framework, char *host, int port) {
	// create server
	wookie_server *server = w_malloc(sizeof *server);
	server->port = port;
	server->address = inet_network(host);
	server->listenfd = -1;
	server->framework = framework;

	memset(&server->settings, '\0', sizeof(server->settings));
	server->settings->on_message_complete = on_message_complete;

	// open the socket
	int on = 1;
	server->listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server->listenfd == -1) {
		printf("ERROR: Could not create socket.");

		close(server->listenfd);
		w_free(server);
		return 1;
	}

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

	// listen with a backlog of 10
	result = listen(server->listenfd, 10);
	if (result == -1) {
		printf("ERROR: %s Could not listen on socket %s:%d\n", strerror(errno), host, server->port);

		close(server->listenfd);
		w_free(server);
		return 1;
	}

	printf("Now listening...\n");

	// get clients
	while (1) {
		// make wookie_client and pass it to desired handler; wookie_handle_client free's the memory
		wookie_client *client = w_malloc(sizeof *client);
		client->connfd = accept(server->listenfd, (struct sockaddr*)NULL, NULL);
		client->server = server;

		// multithreaded handling of clients
		// TODO: don't spawn a new thread with the client waiting :(
		pthread_t thread;
		pthread_create(&thread, NULL, wookie_handle_client, client);
		pthread_detach(thread);
	}

	// close server
	close(server->listenfd);
	w_free(server);
}
