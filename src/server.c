// server.c

// multithreading options (comment multithreading out to go synchronously)
//#define MULTITHREADING
#define MULTITHREADING_THREADS 10

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef MULTITHREADING
#include <pthread.h>
#endif
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// wookie server data representation
typedef struct {
	int port;
	in_addr_t address;
	int listenfd;
	wookie_framework *framework;
} wookie_server;

// wookie client data representation
typedef struct {
	int connfd;
	wookie_server *server;
} wookie_client;

typedef struct {
	wookie_client *client;
	parsed_result *parsed_request;
} wookie_request;

// handles a wookie client (arg is actually a wookie_client instance)
void *wookie_handle_client(void *arg) {
	// get arguments
	wookie_client *client = (wookie_client*)arg;

	// wow, hey, that works
	char *request = calloc(1024, sizeof(char*));
	ssize_t total_read_bytes = 0;

	// MUST WATCH OUT FOR BUFFER OVERFLOW!
	while (1) {
		ssize_t read_bytes = read(client->connfd, &request[total_read_bytes], (1024 * sizeof(char*)) - total_read_bytes);
		total_read_bytes += read_bytes;

		// check if \r\n\r\n or \n\n
		if (request[total_read_bytes - 1] == '\n' && request[total_read_bytes - 3] == '\n'
			&& request[total_read_bytes - 2] == '\r' && request[total_read_bytes - 4] == '\r')
			break;
		else if (request[total_read_bytes - 1] == '\n' && request[total_read_bytes - 2] == '\n')
			break;
	}

	// request contains our HTTP request.
	parsed_result *result = malloc(sizeof *result);
	result = parser_parse(request);

	// free up some now useless memory
	free(request);

	wookie_request *req = malloc(sizeof *req);
	req->client = client;
	req->parsed_request = result;

	// give it back to the framework
	#ifdef MULTITHREADING
	pthread_t thread;
	pthread_create(&thread, NULL, wookie_framework_request, req);
	pthread_detach(thread);
	#else
	wookie_framework_request(req);
	close(client->connfd);
	// free up memory
	free(req->client);
	free(req->parsed_request->path);
	free(req->parsed_request);
	free(req);
	#endif

	// if is multithreaded
	#ifdef MULTITHREADING
	pthread_exit(0);
	#endif

	return NULL;
}

int wookie_start_server(wookie_framework *framework, char *host, int port) {
	// create server
	wookie_server *server = malloc(sizeof *server);
	server->port = port;
	server->address = inet_network(host);
	server->listenfd = -1;
	server->framework = framework;

	// open the socket
	int on = 1;
	server->listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(server->listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	// setup the server address
	struct sockaddr_in server_address;
	memset(&server_address, '\0', sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(server->address);
	server_address.sin_port = htons(server->port);

	// bind to ip
	int result = bind(server->listenfd, (struct sockaddr*)&server_address, sizeof(server_address));

	// check for binding errors
	if (result == -1) {
		printf("ERROR: Could not bind to socket %s:%d\n", host, server->port);
		free(server);
		return 1;
	}

	// backlog of 10
	result = listen(server->listenfd, 10);

	// check for listen errors
	if (result == -1) {
		printf("ERROR: Could not listen on socket %s:%d\n", host, server->port);
		free(server);
		return 1;
	}

	printf("Now listening.\n");

	// get clients
	while (1) {
		// make wookie_client and pass it to desired handler; wookie_handle_client free's the memory
		wookie_client *client = malloc(sizeof *client);
		client->connfd = accept(server->listenfd, (struct sockaddr*)NULL, NULL);
		client->server = server;

		#ifdef MULTITHREADING
		// multithreaded handling of clients
		pthread_t thread;
		pthread_create(&thread, NULL, wookie_handle_client, client);
		pthread_detach(thread);
		#else
		// this handles it in the main thread, which can be a massive issue to performance.
		wookie_handle_client(client);
		free(client);
		#endif
	}

	// close server
	close(server->listenfd);
	free(server);
}
