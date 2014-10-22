// cluster.c
#include <stdio.h>
#include <pthread.h>

#include "util.h"
#include "memory.h"

struct ewok_cluster {
	int workers;
	pthread_t *threads;
};

struct ewok_info {
	void (*function)(void *);
	void *arg;
};

// Private forward declarations
void *cluster_loop(void *arg);


ewok_cluster cluster_init(int workers) {
	if (workers < 1) {
		printf("ERROR: ewok_cluster: cannot create cluster of workers with less than 1 worker; used %d workers.", workers);
	}

	DEBUG("Creating an ewok cluster...");

	ewok_cluster cluster;
	cluster.workers = workers;
	cluster.threads = w_calloc(workers, sizeof(pthread_t));

	return cluster;
}

void cluster_spawn(ewok_cluster cluster, void (function)(void *), void *arg) {
	ewok_info *info = w_malloc(sizeof *info);
	info->function = function;
	info->arg = arg;

	for (int i = 0; i < cluster.workers; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, cluster_loop, info);
		pthread_detach(thread);

		cluster.threads[i] = thread;
	}

	DEBUG("Spawned an ewok cluster");
}

void cluster_stop(ewok_cluster cluster) {
	for (int i = 0; i < cluster.workers; i++) {
		pthread_cancel(cluster.threads[i]);
	}

	DEBUG("Stopped an ewok cluster");
}

void *cluster_loop(void *arg) {
	ewok_info *info = (ewok_info *)arg;

	void (*function)() = info->function;

	while (1) {
		function(info->arg);
		printf("This never actually returns :(\n");
	}
}