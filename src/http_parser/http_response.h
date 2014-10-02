// http_response.
#ifndef HEADER_RESPONSE
#define HEADER_RESPONSE

#include "http_response.c"

/**
 * Sends a response to the client.
 */
void http_response_send(wookie_response *response, int connfd);

#endif