// cluster.c
#include <pthread.h>

void cluster_spawn(void (function)()) {
	pthread_t thread;
	pthread_create(&thread, NULL, cluster_loop, function);
	pthread_detach(thread);
}

void *cluster_loop(void *arg) {
	void (*function)();
	function = &arg;

	while (1) {
		function();
	}
}