// cluster.c
#include <stdio.h>
#include <pthread.h>

#include "util.h"
#include "memory.h"

struct ewok_cluster {
	int workers;
	pthread_t *threads;
};

// Private forward declarations
void *cluster_loop(void *arg);


ewok_cluster *cluster_init(int workers) {
	if (workers < 1) {
		printf("ERROR: ewok_cluster: cannot create cluster of workers with less than 1 worker; used %d workers.", workers);
		return NULL;
	}

	DEBUG("Creating an ewok cluster...");

	ewok_cluster *cluster = w_malloc(sizeof *cluster);
	cluster->workers = workers;
	cluster->threads = w_calloc(workers, sizeof(pthread_t));

	return cluster;
}

void cluster_spawn(ewok_cluster *cluster, void (function)(void *)) {
	for (int i = 0; i < cluster->workers; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, cluster_loop, function);
		pthread_detach(thread);

		cluster->threads[i] = thread;
	}

	DEBUG("Spawned an ewok cluster");
}

void cluster_stop(ewok_cluster *cluster) {
	for (int i = 0; i < cluster->workers; i++) {
		pthread_cancel(cluster->threads[i]);
	}

	w_free(cluster->threads);
	w_free(cluster);

	DEBUG("Stopped an ewok cluster");
}

void *cluster_loop(void *arg) {
	void (*function)();
	function = (void *)arg;

	while (1) {
		function();
	}
}