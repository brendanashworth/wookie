// cluster.h
// Basically, a cluster is a library for creating infinity-looping workers.
#ifndef _CLUSTER_H
#define _CLUSTER_H

void cluster_spawn();

void *cluster_loop(void*);

#include "cluster.c"
#endif