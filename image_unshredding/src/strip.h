#ifndef __STRIP_H__
#define __STRIP_H__

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

namespace co {

    class Strip {
    public:
        int w;
        int h;
        int c;

        std::vector<std::vector<std::vector<int>>> pixels;

        Strip(int w, int h, int c, std::vector<int> &row);

        void print();
    };

    int distance(const Strip &a, const Strip &b);

    std::vector<std::vector<int>> distance_matrix(const std::vector<Strip> &strips);

};

#endif
