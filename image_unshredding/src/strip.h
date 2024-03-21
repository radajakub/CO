#ifndef __STRIP_H__
#define __STRIP_H__

#include <vector>
#include <iostream>
#include <iomanip>

namespace co
{

    class Strip
    {
    public:
        int w;
        int h;
        int c;

        std::vector<std::vector<std::vector<int>>> pixels;

        Strip(int w, int h, int c, std::vector<int> &row);

        void print();
    };

    std::vector<std::vector<std::vector<int>>> vec_to_mat(std::vector<int> &all, int w, int h, int c);

};

#endif
