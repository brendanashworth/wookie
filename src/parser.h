// parser.h
#ifndef _PARSER_H
#define _PARSER_H

typedef struct parsed_result parsed_result;
typedef enum http_version http_version;
typedef enum http_request_type http_request_type;

/**
 * Parses an HTTP request. Returns a heap-allocated object that must be w_free()'d.
 * @param  request the HTTP request in string form
 * @return         the parsed HTTP request in object form
 */
parsed_result *parser_parse(char *request);

/* Parses the request line of the request.
 * Returns a negative number if it wasn't valid HTML.
 */
int parser_parse_requestline(parsed_result *result, char *orig_request);

#include "parser.c"
#endif