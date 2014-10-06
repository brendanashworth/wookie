// server.h
#ifndef _SERVER_H
#define _SERVER_H

/* Start wookie server */
int wookie_start_server(wookie_framework *framework, char *host, int port);

/* Handle wookie client */
void *wookie_handle_client(void *arg);

#include "server.c"
#endif