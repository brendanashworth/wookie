// parser.h
#include "parser.c"

/* Parse request. */
struct parsed_result *parser_parse(char *request);

/* Parses the request line of the request. */
void parser_parse_requestline(struct parsed_result *result, char *orig_request);