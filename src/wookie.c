// wookie.c
#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[]) {
	// must provide host and port
	if (argc != 3) {
		printf("Usage: wookie [host] [port].\n");
		return 1;
	}

	char *host = argv[1];
	int port = atoi(argv[2]);

	printf("Starting wookie HTTP server on %s:%d\n", host, port);

	// go
	int response_code = wookie_start_server(host, port);

	return response_code;
}