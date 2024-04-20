#ifndef __FORD_FULKERSON_H__
#define __FORD_FULKERSON_H__

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "network.h"

bool initialize_flow(Network &network, Flow &flow);

Flow ford_fulkerson(Network &network);

std::vector<int> augmenting_path(Network &net, Flow &flow);

int min_capacity(Network &net, Flow &flow, std::vector<int> &path);

void augment_flow(Network &net, Flow &flow, int delta, std::vector<int> &path);

void ford_fulkerson(Network &network, Flow &flow);

Network zero_bounds(Network &network, std::vector<int> &b);

Network max_flow_from_feasibility(Network &network, std::vector<int> &b);

#endif
