// pthread.c
#include <stdio.h>
#include <pthread.h>
#include "../deps/bench/bench.c"

void *donothing(void *arg) { /* do nothing */ return NULL; }

int main() {
	BENCHMARK(pthread, 3)

	pthread_t thread;
	pthread_create(&thread, NULL, donothing, NULL);
	pthread_detach(thread);

	END_BENCHMARK(pthread)
	BENCHMARK_SUMMARY(pthread);
};