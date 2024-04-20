#include "ford_fulkerson.h"

bool initialize_flow(Network &network, Flow &flow) {
    // transform to 0 lower bounds
    std::vector<int> b(network.V, 0);
    Network feasibility = zero_bounds(network, b);

    feasibility.add_edge(feasibility.t, feasibility.s, 0, INF);

    Network max_flow_net = max_flow_from_feasibility(feasibility, b);
    Flow max_flow(max_flow_net);

    ford_fulkerson(max_flow_net, max_flow);

    // check if all edges are saturated
    for (int ei = 0; ei < max_flow_net.E; ++ei) {
        Edge &e = max_flow_net.edges[ei];
        if (e.start == max_flow_net.s) {
            if (max_flow.f[ei] < e.u) {
                return false;
            }
        }
        if (e.end == max_flow_net.t) {
            if (max_flow.f[ei] < e.u) {
                return false;
            }
        }
    }

    // take only first values of the flow since the edges are appended
    for (int ei = 0; ei < max_flow_net.E; ++ei) {
        Edge &max_edge = max_flow_net.edges[ei];
        for (int ej = 0; ej < network.E; ++ej) {
            Edge &edge = network.edges[ej];
            if (edge.start == max_edge.start - 1 && edge.end == max_edge.end - 1) {
                flow.f[ej] = max_flow.f[ei] + edge.l;
            }
        }
    }
    return true;
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
                sink_found = sink_found || e.end == net.t;
            }
        }

        for (int ei = 0; ei < net.E; ++ei) {
            Edge &e = net.edges[ei];
            if (m[e.end] && !m[e.start] && flow.f[ei] > e.l) {
                m[e.start] = true;
                pred[e.start] = ei;
                changed = true;
                sink_found = sink_found || e.start == net.t;
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
        } else if (current == e.end) {
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

        if (path.size() == 0) {
            break;
        }

        capacity = min_capacity(network, flow, path);
        if (capacity == 0) {
            break;
        }

        augment_flow(network, flow, capacity, path);
    }
}

Network zero_bounds(Network &network, std::vector<int> &b) {
    // make room for new source and sink
    Network zeroed(network);
    for (Edge &e : zeroed.edges) {
        b[e.start] -= e.l;
        b[e.end] += e.l;
        e.u -= e.l;
        e.l = 0;
    }
    return zeroed;
}

Network max_flow_from_feasibility(Network &network, std::vector<int> &b) {
    Network max_flow(network.V + 2);

    // relabel vertices in edges
    for (Edge &e : network.edges) {
        max_flow.add_edge(e.start + 1, e.end + 1, e.l, e.u);
    }

    // add new sink and source
    for (int v = 0; v < network.V; ++v) {
        if (b[v] > 0) {
            max_flow.add_edge(max_flow.s, v + 1, 0, b[v]);
        } else if (b[v] < 0) {
            max_flow.add_edge(v + 1, max_flow.t, 0, -b[v]);
        }
    }

    return max_flow;
}
