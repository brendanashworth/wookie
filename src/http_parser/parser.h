// parser.h
#ifndef HEADER_PARSER
#define HEADER_PARSER

#include "parser.c"

//enum http_request_type;
//enum http_version;
//struct parsed_result;

/* Parse request. */
parsed_result *parser_parse(char *request);

/* Parses the request line of the request.
 * Returns a negative number if it wasn't valid HTML.
 */
int parser_parse_requestline(parsed_result *result, char *orig_request);

#endif