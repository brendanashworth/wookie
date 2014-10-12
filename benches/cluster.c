// cluster.c
#include <stdio.h>
#include <unistd.h>
#include "../deps/bench/bench.c"
#include "../src/cluster.h"

void *donothing(void *arg) {
	usleep(1);
	return NULL;
}

int main() {
	{
		// First measure ewok_cluster with 512 ewoks
		MEASURE(cluster_512)

		ewok_cluster *cluster = cluster_init(512);
		cluster_spawn(cluster, &donothing);

		cluster_stop(cluster);

		END_MEASURE(cluster_512)
		MEASURE_SUMMARY(cluster_512);
	}

	{
		// Now measure ewok_cluster with 128 ewoks (1/4)
		MEASURE(cluster_128)

		ewok_cluster *cluster = cluster_init(128);
		cluster_spawn(cluster, &donothing);

		cluster_stop(cluster);

		END_MEASURE(cluster_128)
		MEASURE_SUMMARY(cluster_128);
	}

	{
		// Now measure ewok_cluster with 16 ewoks (1/32)
		MEASURE(cluster_16)

		ewok_cluster *cluster = cluster_init(16);
		cluster_spawn(cluster, &donothing);

		cluster_stop(cluster);

		END_MEASURE(cluster_16)
		MEASURE_SUMMARY(cluster_16);
	}
};