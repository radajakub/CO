#include "network.h"

void Edge::println() {
    std::cout << this->start << "-(" << this->l << ", " << this->f << ", " << this->u << ")->" << this->end << std::endl;
}

Network::Network(std::string &path) {
    std::ifstream f(path, std::ios_base::in);

    std::string line;
    std::getline(f, line);
    std::istringstream lstream(line);

    lstream >> this->C >> this->P;

    // prepare vertex numbers
    this->s = 0;
    this->cs.resize(this->C);
    std::iota(this->cs.begin(), this->cs.end(), 1);
    this->ps.resize(this->P);
    std::iota(this->ps.begin(), this->ps.end(), C + 1);
    this->t = this->C + this->P + 1;

    this->edges.resize(this->C + this->P + 2);

    int l, u, val;

    for (int i = 0; i < this->C; ++i) {
        std::getline(f, line);
        lstream = std::istringstream(line);

        // load l and u
        lstream >> l >> u;

        // add edge leading from s to customer node with capacity [l, u]
        this->edges[s].push_back(Edge(this->s, this->cs[i], l, u));

        // load purchased products
        while (lstream >> val) {
            // add edge leading from customer node to product with capacity [0, 1]
            int start = this->cs[i];
            int end = this->ps[val - 1];
            this->edges[start].push_back(Edge(start, end, 0, 1));
        }
    }

    std::getline(f, line);
    lstream = std::istringstream(line);
    for (int i = 0; i < this->P; ++i) {
        lstream >> val;
        int prod = this->ps[i];
        this->edges[prod].push_back(Edge(prod, this->t, val, std::numeric_limits<int>::max()));
    }

    f.close();
}

void Network::print() {
    std::cout << "C: " << this->C << std::endl;
    std::cout << "P: " << this->P << std::endl;
    std::cout << "s: " << this->s << std::endl;
    std::cout << "t: " << this->t << std::endl;

    std::cout << "cs:";
    for (int i = 0; i < this->cs.size(); ++i) {
        std::cout << " " << this->cs[i];
    }
    std::cout << std::endl;
    std::cout << "ps:";
    for (int i = 0; i < this->ps.size(); ++i) {
        std::cout << " " << this->ps[i];
    }
    std::cout << std::endl;

    for (std::vector<Edge> &es : this->edges) {
        for (Edge &e : es) {
            e.println();
        }
    }
}
