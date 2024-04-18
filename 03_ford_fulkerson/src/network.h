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
    int f;
    int u;

    Edge(int start, int end, int l, int u) : start(start), end(end), l(l), f(0), u(u) {}

    void println();
};

class Network {
public:
    int P;
    int C;

    int s;
    int t;
    std::vector<int> cs;
    std::vector<int> ps;

    std::vector<std::vector<Edge>> edges;

    Network(std::string &path);

    void print();
};

#endif
