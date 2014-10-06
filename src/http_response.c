// http_response.c
#include <sys/socket.h>

struct wookie_response {
	char *code;
	char *content;
};

/**
 * End a header (or entire HTTP header) of a response.
 * @param connfd Connection file descriptor.
 */
static inline void http_response_end_header(int connfd) {
	send(connfd, "\r\n", 2, 0);
}

void http_response_send(wookie_response *response, int connfd) {
	// First send response status
	http_response_send_statusline("HTTP/1.1", response->code, "OK", connfd);

	http_response_send_header("Content-Length", "xyz", connfd);
	http_response_send_header("Server", "wookie", connfd);
	http_response_end_header(connfd);

	// Response
	send(connfd, response->content, strlen(response->content), 0);
}

void http_response_send_statusline(char *version, char *code, char *message, int connfd) {
	// Calculate the length of the status line
	size_t length = strlen(version) + 1 + strlen(code) + 1 + strlen(message) + 2 + 1;

	// Allocate and build the status line
	char *statusline = w_malloc(length);
	strcpy(statusline, version);
	strcat(statusline, " ");
	strcat(statusline, code);
	strcat(statusline, " ");
	strcat(statusline, message);
	strcat(statusline, "\r\n");

	// Send the status line
	send(connfd, statusline, length, 0);
}

void http_response_send_header(char *key, char *value, int connfd) {
	// Calculate length of header
	size_t length = strlen(key) + 2 + strlen(value) + 1;

	char *header = w_malloc(length);
	strcpy(header, key);
	strcat(header, ": ");
	strcat(header, value);

	// Send actual header
	send(connfd, header, length, 0);

	// Send line endings
	http_response_end_header(connfd);
}
