// http_response.
#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H

typedef struct wookie_response wookie_response;

/**
 * Sends an HTTP response to a client.
 * @param response The response.
 * @param connfd   Connection file descriptor of the client.
 */
void http_response_send(wookie_response *response, int connfd);

#include "http_response.c"
#endif