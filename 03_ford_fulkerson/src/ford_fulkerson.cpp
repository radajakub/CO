#include "ford_fulkerson.h"

void initialize_flow(Network &network, Flow &f) {
    Network extended(network);
    extended.add_edge(extended.t, extended.s, 0, INF);
}

std::vector<int> augmenting_path(Network &net, Flow &flow) {
    std::vector<int> path;
    std::unordered_map<int, int> pred;

    std::vector<bool> m(net.V, false);
    m[net.s] = true;

    bool changed = true;
    bool sink_found = false;
    while (changed) {
        changed = false;

        // check forward
        for (int ei = 0; ei < net.E; ++ei) {
            Edge &e = net.edges[ei];
            if (m[e.start] && !m[e.end] && flow.f[ei] < e.u) {
                m[e.end] = true;
                pred[e.end] = ei;
                changed = true;
                sink_found = e.end == net.t;
            }
        }

        // check backward
        for (int ei = 0; ei < net.E; ++ei) {
            Edge &e = net.edges[ei];
            if (m[e.end] && !m[e.start] && flow.f[ei] > e.l) {
                m[e.start] = true;
                pred[e.start] = ei;
                changed = true;
                sink_found = e.end == net.t;
            }
        }

        // check sink
        if (sink_found) {
            int curr = net.t;
            while (curr != net.s) {
                int ei = pred[curr];
                path.push_back(ei);
                Edge &e = net.edges[ei];
                curr = e.start == curr ? e.end : e.start;
            }
            std::reverse(path.begin(), path.end());
            break;
        }
    }

    return path;
}

int min_capacity(Network &net, Flow &flow, std::vector<int> &path) {
    int current = net.s;
    int min_capacity = INF;
    for (int ei : path) {
        Edge &e = net.edges[ei];
        if (current == e.start) {
            min_capacity = std::min(min_capacity, e.u - flow.f[ei]);
            current = e.end;
        } else {
            min_capacity = std::min(min_capacity, flow.f[ei] - e.l);
            current = e.start;
        }
    }
    return min_capacity;
}

void augment_flow(Network &net, Flow &flow, int delta, std::vector<int> &path) {
    int current = net.s;
    for (int ei : path) {
        Edge &e = net.edges[ei];
        if (current == e.start) {
            flow.f[ei] += delta;
            current = e.end;
        } else {
            flow.f[ei] -= delta;
            current = e.start;
        }
    }
}

void ford_fulkerson(Network &network, Flow &flow) {
    std::vector<int> path;
    int capacity;
    while (true) {
        path = augmenting_path(network, flow);

        // std::cout << "augmenting path:" << std::endl;
        // for (int ei : path) {
        //     network.edges[ei].println();
        // }

        if (path.size() == 0) {
            // std::cout << "no path -> end" << std::endl;
            break;
        }

        capacity = min_capacity(network, flow, path);
        // std::cout << "min capacity: " << capacity << std::endl
        //           << std::endl;

        augment_flow(network, flow, capacity, path);
    }
}
