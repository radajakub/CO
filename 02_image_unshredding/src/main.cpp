#include <gurobi_c++.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

#include "strip.h"

void print_vector(const std::vector<int> &v) {
    for (int i : v) std::cout << i << " ";
    std::cout << std::endl;
}

void print_vector_indexed(const std::vector<int> &v) {
    for (int i = 0; i < v.size(); ++i) std::cout << i << " -> " << v[i] << std::endl;
}

std::vector<int> find_targets(std::vector<std::vector<int>> &edges) {
    int n = edges.size();
    std::vector<int> targets(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (edges[i][j] > 0) {
                targets[i] = j;
            }
        }
    }

    return targets;
}

std::vector<int> find_cycle(std::vector<int> &targets, int initial) {
    int n = targets.size();
    // reserve vector for cycle starting from initial
    std::vector<int> S;
    S.reserve(n);

    int current = initial;

    for (int i = 0; i < n; ++i) {
        S.push_back(current);
        current = targets[current];
        if (current == initial) {
            break;
        }
    }
    return S;
}

class cycle_check : public GRBCallback {
public:
    int n;
    GRBVar **x;
    cycle_check(int n, GRBVar **x) : n(n), x(x) {}

protected:
    void callback() {
        if (where == GRB_CB_MIPSOL) {
            // (1) get all variable solutions
            std::vector<std::vector<int>> sol(n, std::vector<int>(n, 0));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i == j) continue;
                    double val = getSolution(x[i][j]);
                    if (val > 0.5) {
                        sol[i][j] = 1;
                        break;
                    }
                }
            }

            // find a target node j for each node i
            // i.e. targets[i] = j means i -> j
            std::vector<int> targets = find_targets(sol);

            // prepare to find all cycles and select the shortest one
            std::vector<bool> all(n, true);
            // iterate through all and mark those visited
            // if visited, skip
            int smallest = n + 1;
            std::vector<int> S;
            for (int i = 0; i < n; ++i) {
                if (!all[i]) {
                    continue;
                }

                std::vector<int> cycle = find_cycle(targets, i);

                int cycle_size = cycle.size();
                if (cycle_size < smallest) {
                    smallest = cycle_size;
                    S = cycle;
                }
                for (int j : cycle) {
                    all[j] = false;
                }
            }

            int S_size = S.size();
            // if it is not the whole cycle, add constraint
            if (S_size < n) {
                GRBLinExpr expr = 0;
                for (int i = 0; i < S_size; ++i) {
                    for (int j = 0; j < S_size; ++j) {
                        if (i != j) {
                            expr += x[S[i]][S[j]];
                        }
                    }
                }
                addLazy(expr <= (S_size - 1));
            }
        }
    }
};

std::vector<int> solve(std::vector<std::vector<int>> &d) {
    int n = d.size();

    GRBEnv env;
    GRBModel model(env);

    // turn on lazy constraints
    model.set(GRB_IntParam_LazyConstraints, 1);

    GRBVar **x = new GRBVar *[n];
    for (int i = 0; i < n; ++i)
        x[i] = new GRBVar[n];

    // add binary variable lb=0.0, ub=1.0, coefficient for each edge is distance d[i][j]
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            x[i][j] = model.addVar(0.0, i == j ? 0.0 : 1.0, d[i][j], GRB_BINARY);

    // add constraint (2) -> only one incoming edge for each vertex
    for (int j = 0; j < n; ++j) {
        GRBLinExpr col_sum = 0;
        for (int i = 0; i < n; ++i) {
            col_sum += x[i][j];
        }
        model.addConstr(col_sum == 1);
    }

    // add constraint (3) -> only one outgoing edge for each vertex
    for (int i = 0; i < n; ++i) {
        GRBLinExpr row_sum = 0;
        for (int j = 0; j < n; ++j) {
            row_sum += x[i][j];
        }
        model.addConstr(row_sum == 1);
    }

    // set callback for lazy constraints
    cycle_check cb(n, x);
    model.setCallback(&cb);

    model.optimize();

    // extract solutions to matrix for better work
    std::vector<std::vector<int>> sol(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double val = x[i][j].get(GRB_DoubleAttr_X);
            if (val > 0.5) {
                sol[i][j] = 1;
            }
        }
    }

    // free variables
    delete[] x;

    // extract non-zero column for each row
    std::vector<int> targets = find_targets(sol);

    // find cycle from the dummy node
    std::vector<int> path = find_cycle(targets, n - 1);

    return path;
}

std::vector<co::Strip> load(const std::string &path) {
    std::ifstream f(path, std::ios_base::in);
    int n, w, h;
    int c = 3;

    std::vector<co::Strip> strips;
    strips.reserve(n);

    f >> n >> w >> h;

    for (int i = 0; i < n; ++i) {
        std::vector<int> row(w * h * c);
        for (int j = 0; j < w * h * c; ++j) {
            f >> row[j];
        }
        strips.push_back(co::Strip(w, h, c, row));
    }

    f.close();

    return strips;
}

void save(std::vector<int> solution, const std::string &path) {
    std::ofstream f(path, std::ios_base::out);
    for (int i = 1; i < solution.size(); ++i) {
        f << solution[i] + 1 << " ";
    }
    f << std::endl;
    f.close();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::vector<co::Strip> strips = load(input_path);

    // distance matrix adds dummy zero row and column to the end
    std::vector<std::vector<int>> distances = co::distance_matrix(strips);

    std::vector<int> path = solve(distances);

    save(path, output_path);

    return 0;
}
