#ifndef __BRATLEY_H__
#define __BRATLEY_H__

#include <algorithm>
#include <limits>
#include <numeric>
#include <stack>
#include <vector>

#include "tasks.h"

class Node {
public:
    int c;
    std::vector<int> scheduled;
    std::vector<int> unscheduled;

    Node(int c, std::vector<int> scheduled, std::vector<int> unscheduled) : c(c), scheduled(scheduled), unscheduled(unscheduled){};

    bool rule1(Tasks &tasks);
    bool rule2(Tasks &tasks, int ub);
    bool rule3(Tasks &tasks);

    void print();
    void print_solution(std::fstream &f, Tasks &tasks);
};

Node bratley(Tasks &tasks);

#endif
