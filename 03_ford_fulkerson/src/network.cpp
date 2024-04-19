#include "network.h"

void Edge::println() {
    std::cout << this->start << "-(" << this->l << "|_|" << this->u << ")->" << this->end << std::endl;
}

void Edge::println(int f) {
    std::cout << this->start << "-(" << this->l << "|" << f << "|" << this->u << ")->" << this->end << std::endl;
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
        this->add_edge(this->ps[i], this->t, vi, INF);
    }

    f.close();
}

Network::Network(int V) : V(V) {
    this->C = -1;
    this->P = -1;
    this->cs = {};
    this->ps = {};

    this->s = 0;
    this->t = V - 1;
    this->E = 0;
    this->edges.reserve(((this->V - 1) * this->V) / 2);
}

void Network::add_edge(int start, int end, int l, int u) {
    this->edges.push_back(Edge(start, end, l, u));
    this->E++;
}

bool Network::need_initial() {
    for (Edge &e : this->edges)
        if (e.l != 0)
            return true;
    return false;
}

void Network::_print_start() {
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
}

void Network::print() {
    this->_print_start();
    std::cout << "Edges:" << std::endl;
    for (int i = 0; i < this->E; ++i) {
        std::cout << i << ": ";
        this->edges[i].println();
    }
    std::cout << std::endl;
}

void Network::print(Flow &flow) {
    this->_print_start();
    std::cout << "Edges:" << std::endl;
    for (int e = 0; e < this->E; ++e) {
        std::cout << e << ": ";
        this->edges[e].println(flow.f[e]);
    }
    std::cout << std::endl;
}

bool Flow::check_feasible(Network &network) {
    for (int e = 0; e < network.E; ++e)
        if (this->f[e] < network.edges[e].l || this->f[e] > network.edges[e].u)
            return false;
    return true;
}
