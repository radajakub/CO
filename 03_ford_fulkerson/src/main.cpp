#include <fstream>
#include <iostream>
#include <string>

#include "network.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string in_file = argv[1];
    std::string out_file = argv[2];

    Network network(in_file);
    network.print();

    return 0;
}