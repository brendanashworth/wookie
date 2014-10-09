// cluster.h
// Basically, a cluster is a library for creating infinity-looping workers.
#ifndef _CLUSTER_H
#define _CLUSTER_H

typedef struct ewok_cluster ewok_cluster;

void cluster_spawn(void (function)());

void *cluster_loop(void *arg);

#include "cluster.c"
#endif