#include "tasks.h"

Tasks::Tasks(std::string &in_path) {
    std::ifstream f(in_path, std::ios_base::in);

    f >> this->n;
    this->r.resize(this->n);
    this->p.resize(this->n);
    this->d.resize(this->n);

    for (int i = 0; i < this->n; i++) {
        f >> this->p[i] >> this->r[i] >> this->d[i];
    }

    f.close();
}

void Tasks::print() {
    std::cout << "n: " << this->n << std::endl;
    for (int i = 0; i < this->n; i++) {
        std::cout << this->p[i] << " " << this->r[i] << " " << this->d[i] << std::endl;
    }
}
