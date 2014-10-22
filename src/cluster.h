// cluster.h
// Basically, a cluster is a library for creating infinity-looping workers.
#ifndef _CLUSTER_H
#define _CLUSTER_H

typedef struct ewok_cluster ewok_cluster;
typedef struct ewok_info ewok_info;

/**
 * Creates an ewok cluster.
 * @param  workers Amount of workers in the cluster.
 * @return         The ewok cluster.
 */
ewok_cluster cluster_init(int workers);

void cluster_spawn(ewok_cluster cluster, void (function)(void *), void *arg);

/**
 * Stops a cluster.
 * @param cluster The ewok cluster to stop.
 */
void cluster_stop(ewok_cluster cluster);

#include "cluster.c"
#endif