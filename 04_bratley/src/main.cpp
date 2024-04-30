#include <iostream>
#include <string>

#include "bratley.h"
#include "tasks.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string in_file = argv[1];
    std::string out_file = argv[2];

    Tasks tasks(in_file);

    Node result = bratley(tasks);

    // print result
    std::fstream f(out_file, std::ios_base::out);
    if (result.c == 0) {
        f << "-1" << std::endl;
    } else {
        result.print_solution(f, tasks);
    }

    f.close();

    return 0;
}
