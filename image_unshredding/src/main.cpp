#include <gurobi_c++.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "strip.h"

int solve()
{
    GRBEnv env;
    GRBModel model(env);

    // GRBVar *x = model.addVars(g.E, GRB_BINARY);

    // set objective as a sum of edge costs for edges which are in the graph
    // model.setObjective(0, GRB_MINIMIZE);

    model.optimize();

    int obj_value = model.get(GRB_DoubleAttr_ObjVal);

    // free variables
    // delete[] x;

    return obj_value;
}

std::vector<co::Strip> load(const std::string &path)
{

    std::ifstream f(path, std::ios_base::in);
    int n, w, h;
    int c = 3;

    std::vector<co::Strip> strips;
    strips.reserve(n);

    f >> n >> w >> h;

    for (int i = 0; i < n; ++i)
    {
        std::vector<int> row(w * h * c);
        for (int j = 0; j < w * h * c; ++j)
        {
            f >> row[j];
        }
        strips.push_back(co::Strip(w, h, c, row));
    }

    return strips;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::vector<co::Strip> strips = load(input_path);

    std::vector<std::vector<int>> distances = co::distance_matrix(strips);

    for (co::Strip &strip : strips)
    {
        strip.print();
        std::cout << std::endl;
    }

    for (int i = 0; i < distances.size(); ++i)
    {
        for (int j = 0; j < distances[i].size(); ++j)
        {
            std::cout << std::setw(3) << std::setfill(' ') << distances[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
