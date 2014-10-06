// http_response.c
#include <sys/socket.h>

struct wookie_response {
	int code;
	char *content;
};

void http_response_send(wookie_response *response, int connfd) {
	//char *format = "HTTP/1.1 %i http response message here\r\nContent-Length: %lu\r\n\r\n%s\r\n";
	char *message = w_malloc(strlen(format));
	sprintf(message, "HTTP/1.1 %i HTTP Message here\r\nContent-Length: %lu\r\n\r\n%s\r\n", response->code, strlen(response->content), response->content);

	send(connfd, message, strlen(message), 0);
}
