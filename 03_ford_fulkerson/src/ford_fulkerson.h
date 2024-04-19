#ifndef __FORD_FULKERSON_H__
#define __FORD_FULKERSON_H__

#include "network.h"

void initialize_flow(Network &network, Flow &f);

Flow ford_fulkerson(Network &network);

std::vector<int> augmenting_path(Network &net, Flow &flow);

int min_capacity(Network &net, Flow &flow, std::vector<int> &path);

void augment_flow(Network &net, Flow &flow, int delta, std::vector<int> &path);

void ford_fulkerson(Network &network, Flow &flow);

#endif
