#include <fstream>
#include <iostream>
#include <string>

#include "ford_fulkerson.h"
#include "network.h"

void test() {
    Network net(5);
    net.add_edge(0, 1, 0, 5);
    net.add_edge(0, 2, 0, 7);
    net.add_edge(0, 3, 0, 3);
    net.add_edge(1, 4, 0, 6);
    net.add_edge(2, 1, 0, 2);
    net.add_edge(2, 4, 0, 5);
    net.add_edge(3, 2, 0, 4);
    net.add_edge(3, 4, 0, 8);
    Flow f(net);
    net.print(f);

    ford_fulkerson(net, f);

    net.print(f);
}

void test_ford_fulkerson() {
    Network net(8);
    net.add_edge(0, 1, 0, 4);
    net.add_edge(0, 2, 2, 5);
    net.add_edge(0, 3, 0, 7);
    net.add_edge(1, 4, 0, 7);
    net.add_edge(2, 4, 0, 6);
    net.add_edge(2, 5, 1, 4);
    net.add_edge(2, 6, 0, 1);
    net.add_edge(3, 5, 0, 8);
    net.add_edge(3, 6, 0, 1);
    net.add_edge(4, 7, 0, 7);
    net.add_edge(5, 7, 0, 6);
    net.add_edge(6, 7, 1, 4);
    Flow flow(net);
    flow.f = {4, 5, 4, 4, 1, 3, 1, 3, 1, 5, 6, 2};

    net.print(flow);
    ford_fulkerson(net, flow);
    net.print(flow);
}

void test_initial() {
    Network net(4);
    net.add_edge(0, 1, 1, 3);
    net.add_edge(0, 2, 2, 3);
    net.add_edge(1, 3, 2, 4);
    net.add_edge(2, 1, 1, 3);
    net.add_edge(2, 3, 0, 2);

    Flow f(net);
    initialize_flow(net, f);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string in_file = argv[1];
    std::string out_file = argv[2];

    Network network(in_file);
    Flow flow(network);

    if (network.need_initial()) {
        bool feasible = initialize_flow(network, flow);
        if (!feasible) {
            std::ofstream f(out_file, std::ios_base::out);
            f << "-1" << std::endl;
            f.close();
            return 0;
        }
    }

    ford_fulkerson(network, flow);

    network.save_solution(flow, out_file);

    return 0;
}
