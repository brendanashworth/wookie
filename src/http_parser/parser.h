// parser.h
#include "parser.c"

enum http_request_type;
enum http_version;
struct parsed_result;

/* Parse request. */
struct parsed_result *parser_parse(char *request);

/* Parses the request line of the request.
 * Returns a negative number if it wasn't valid HTML.
 */
int parser_parse_requestline(struct parsed_result *result, char *orig_request);