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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string in_file = argv[1];
    std::string out_file = argv[2];

    Network network(in_file);
    network.print();

    Flow flow(network);

    if (network.need_initial()) {
        initialize_flow(network, flow);
        std::cout << "need init" << std::endl;
    }

    ford_fulkerson(network, flow);

    network.print(flow);

    // bool feasible = flow.check_feasible(network);
    // std::cout << "Feasible: " << feasible << std::endl;

    return 0;
}
