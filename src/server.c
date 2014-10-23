// server.c

// multithreading options (comment multithreading out to go synchronously)
#define EWOK_WORKERS 4

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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

// Private forward declarations
void *wookie_server_work(wookie_server *server);

// Callback for the HTTP parser, simply forwards to wookie_framework_request
int on_message_complete(http_parser *parser, const char *at, size_t length) {
	// Pass the request to the framework
	wookie_framework_request(parser);

	return 0;
}

// handles a wookie client
void wookie_handle_client(wookie_client *client) {
	http_parser_settings settings;
	memset(&settings, '\0', sizeof(http_parser_settings));
	settings.on_message_complete = on_message_complete;

	http_parser *parser = w_malloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = (void *)client;

	size_t len = 80 * 1024;
	char buf[len];

	ssize_t recved = recv(client->connfd, buf, len, 0);
	if (recved < 0) {
		// Error
		close(client->connfd);
		w_free(parser);
		return;
	}

	size_t nparsed = http_parser_execute(parser, &settings, buf, recved);

	// did we get the same amount of bytes?
	if ((unsigned)recved != nparsed) {
		// Error
		close(client->connfd);
		w_free(parser);
		return;
	}

	// Cleanup
	close(client->connfd);
	w_free(parser);

	// the http parser will then call on_message_complete() with the appropriate data
	return;
}

int wookie_start_server(wookie_framework *framework, char *host, int port) {
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

	printf("Now listening on socket...\n");

	// Create the cluster
	ewok_cluster cluster = cluster_init(EWOK_WORKERS);
	cluster_spawn(cluster, &wookie_server_work, server);

	// Now we nonchalantly wait until forever. Stops are handled from SIGINT
	while (1) {
		sleep(1); // Wait one second
	}
}

void *wookie_server_work(wookie_server *server) {
	wookie_client *client = w_malloc(sizeof *client);
	client->server = server;

	client->connfd = accept(server->listenfd, (struct sockaddr*)NULL, NULL);

	// Handle eror
	if (client->connfd < 0) {
		printf("Error accept()ing client: %d\n", errno);
		return NULL;
	}

	// Now that we've accepted a new client, process the client.
	wookie_handle_client(client);

	// The function returned and we can go back into the cluster :)
	return NULL;
}
