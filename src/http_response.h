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

/**
 * Sends an HTTP response header to a client.
 * @param key    The key of the header.
 * @param value  The value of the header.
 * @param connfd Connection file descriptor of the client.
 */
void http_response_send_header(char *key, char *value, int connfd);

/**
 * Send an HTTP response status line to a client.
 * @param version The HTTP version.
 * @param code    The response status code.
 * @param message The response status code's message.
 * @param connfd  The client's connection file descriptor.
 */
void http_response_send_statusline(char *version, char *code, char *message, int connfd);

#include "http_response.c"
#endif