// server.h
#include "server.c"

struct wookie_server;
struct wookie_client;

/* Start wookie server */
int wookie_start_server(char *host, int port);

/* Handle wookie client */
void *wookie_handle_client(void *arg);