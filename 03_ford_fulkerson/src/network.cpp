#include "network.h"

void Edge::println() {
    std::cout << this->start << "-(" << this->l << "|_|" << this->u << ")->" << this->end << std::endl;
}

Network::Network(std::string &path) {
    std::ifstream f(path, std::ios_base::in);

    std::string line;
    std::getline(f, line);
    std::istringstream lstream(line);

    lstream >> this->C >> this->P;

    this->V = this->C + this->P + 2;
    this->E = 0;

    // prepare vertex numbers
    this->s = 0;
    this->cs.resize(this->C);
    std::iota(this->cs.begin(), this->cs.end(), 1);
    this->ps.resize(this->P);
    std::iota(this->ps.begin(), this->ps.end(), C + 1);
    this->t = this->C + this->P + 1;

    this->adjacency.resize(this->V);
    this->edges.reserve(((this->V - 1) * this->V) / 2);

    int l, u, p;
    for (int i = 0; i < this->C; ++i) {
        std::getline(f, line);
        lstream = std::istringstream(line);

        // load l and u
        lstream >> l >> u;

        int ci = this->cs[i];

        // add edge leading from source s to customer ci with capacity [l, u]
        this->add_edge(this->s, ci, l, u);

        // load purchased products of customer ci
        while (lstream >> p) {
            // add edge leading from customer ci to product p with capacity [0, 1]
            this->add_edge(ci, this->ps[p - 1], 0, 1);
        }
    }

    std::getline(f, line);
    lstream = std::istringstream(line);
    int vi;
    for (int i = 0; i < this->P; ++i) {
        lstream >> vi;
        // add edge leading from product pi to sink t with capacity [vi, inf)
        this->add_edge(this->ps[i], this->t, vi, std::numeric_limits<int>::max());
    }

    f.close();
}

void Network::add_edge(int start, int end, int l, int u) {
    this->edges.push_back(Edge(start, end, l, u));
    this->adjacency[start].push_back(this->E);
    this->E++;
}

void Network::print() {
    std::cout << "G = (" << this->V << "," << this->E << ")" << std::endl;
    std::cout << std::endl;

    std::cout << "C: " << this->C << std::endl;
    std::cout << "P: " << this->P << std::endl;
    std::cout << std::endl;

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
    std::cout << std::endl;

    std::cout << "Edges:" << std::endl;
    for (int i = 0; i < this->E; ++i) {
        std::cout << i << ": ";
        this->edges[i].println();
    }
    std::cout << std::endl;

    std::cout << "Adjacency:" << std::endl;
    for (int v = 0; v < this->V; ++v) {
        std::cout << v << ":";
        for (int e : this->adjacency[v]) {
            std::cout << " " << e;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
