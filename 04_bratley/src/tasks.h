#ifndef __TASKS_H__
#define __TASKS_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Tasks {
public:
    int n;
    std::vector<int> r;
    std::vector<int> p;
    std::vector<int> d;

    Tasks(std::string &in_path);

    void print();
};

#endif
