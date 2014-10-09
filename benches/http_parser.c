// http_parser.c
#include <stdio.h>
#include "../src/parser.h"
#include "../deps/bench/bench.c"

static char *request = "GET /path/1 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomBencher\r\nAccept: text/html\r\n\r\n";

int main() {
	BENCHMARK(http_parser, 3)

	// http parsing stuff
	parsed_result *result = parser_parse(request);
	w_free(result);

	END_BENCHMARK(http_parser)
	BENCHMARK_SUMMARY(http_parser);
};