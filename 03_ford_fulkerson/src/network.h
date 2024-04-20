#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// const int INF = std::numeric_limits<int>::max();
const int INF = 1000;

class Edge;
class Network;
class Flow;

class Edge {
public:
    int start;
    int end;
    int l;
    int u;

    Edge(int start, int end, int l, int u) : start(start), end(end), l(l), u(u) {}

    void println();
    void println(int f);
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

    Network(std::string &path);
    Network(int V);

    void add_edge(int start, int end, int l, int u);

    bool need_initial();

    void save_solution(Flow &flow, std::string &path);

    void _print_start();
    void print();
    void print(Flow &flow);
};

class Flow {
public:
    std::vector<int> f;

    Flow(Network &network) : f(std::vector<int>(network.E, 0)) {}

    bool check_feasible(Network &network);
};

#endif
