#include "strip.h"

co::Strip::Strip(int w, int h, int c, std::vector<int> &row)
{
    this->w = w;
    this->h = h;
    this->c = c;

    this->pixels = co::vec_to_mat(row, w, h, c);
}

void co::Strip::print()
{
    for (int i = 0; i < this->h; ++i)
    {
        for (int j = 0; j < this->w; ++j)
        {
            std::cout << "(";
            for (int k = 0; k < this->c; ++k)
            {
                if (k)
                {
                    std::cout << ", ";
                }
                std::cout << std::setw(3) << std::setfill(' ') << this->pixels[i][j][k];
            }
            std::cout << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<std::vector<int>>> co::vec_to_mat(std::vector<int> &all, int w, int h, int c)
{
    std::vector<std::vector<std::vector<int>>> mat(h, std::vector<std::vector<int>>(w, std::vector<int>(c, 0)));

    int i = 0;
    int j = 0;
    int k = 0;
    for (int n = 0; n < all.size(); ++n)
    {
        mat[i][j][k] = all[n];
        ++k;
        if (k == 3)
        {
            k = 0;
            ++j;
            if (j == w)
            {
                j = 0;
                ++i;
            }
        }
    }

    return mat;
}