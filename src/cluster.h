// cluster.h
// Basically, a cluster is a library for creating infinity-looping workers.
#ifndef HEADER_CLUSTER
#define HEADER_CLUSTER

void cluster_spawn();

void *cluster_loop(void*);

#include "cluster.c"

#endif