// server.h
#ifndef HEADER_SERVER
#define HEADER_SERVER

#include "server.c"

/* Start wookie server */
int wookie_start_server(char *host, int port);

/* Handle wookie client */
void *wookie_handle_client(void *arg);

#endif