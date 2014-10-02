// http_response.c
#include <sys/socket.h>

typedef struct {
	int code;
	char *content;
} wookie_response;

void http_response_send(wookie_response *response, int connfd) {
	char *message = NULL;
	sprintf(message, "HTTP/1.1 %i HTTP Message here\r\nContent-Length: %lu\r\n\r\n%s\r\n", response->code, strlen(response->content), response->content);

	send(connfd, message, strlen(message), 0);
}