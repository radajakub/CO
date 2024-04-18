#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

class Edge {
public:
    int start;
    int end;
    int l;
    int u;

    Edge(int start, int end, int l, int u) : start(start), end(end), l(l), u(u) {}

    void println();
};

class Network {
public:
    int V;  // total number of vertices
    int E;  // total number of edges

    int P;  // number of products
    int C;  // number of customers

    int s;                // source node
    int t;                // sink node
    std::vector<int> cs;  // customer nodes
    std::vector<int> ps;  // product nodes

    std::vector<Edge> edges;  // list of all edges

    std::vector<std::vector<int>> adjacency;  // adjacency list

    Network(std::string &path);

    void add_edge(int start, int end, int l, int u);

    void print();
};

#endif
