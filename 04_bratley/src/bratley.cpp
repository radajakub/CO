#include "bratley.h"

bool Node::rule1(Tasks &tasks) {
    for (int j : this->unscheduled) {
        int rj = tasks.r[j];
        int pj = tasks.p[j];
        int dj = tasks.d[j];
        if (std::max(this->c, rj) + pj > dj) {
            return true;
        }
    }
    return false;
}

bool Node::rule2(Tasks &tasks, int ub) {
    int min_rj = std::numeric_limits<int>::max();
    for (int j : this->unscheduled) {
        min_rj = std::min(min_rj, tasks.r[j]);
    }
    int lb = std::max(this->c, min_rj);
    for (int j : this->unscheduled) {
        lb += tasks.p[j];
    }

    return lb >= ub;
}

bool Node::rule3(Tasks &tasks) {
    int min_rj = std::numeric_limits<int>::max();
    for (int j : this->unscheduled) {
        min_rj = std::min(min_rj, tasks.r[j]);
    }
    return this->c <= min_rj;
}

void Node::print() {
    std::cout << this->c << " {";
    for (int i = 0; i < this->scheduled.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->scheduled[i];
    }
    std::cout << "} {";
    for (int i = 0; i < this->unscheduled.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << this->unscheduled[i];
    }
    std::cout << "}" << std::endl;
}

void Node::print_solution(std::fstream &f, Tasks &tasks) {
    std::vector<int> s(tasks.n, 0);
    int si = 0;
    for (int ti : this->scheduled) {
        si = std::max(si, tasks.r[ti]);
        s[ti] = si;
        si += tasks.p[ti];
    }

    for (int i = 0; i < tasks.n; i++) {
        f << s[i] << std::endl;
    }
}

Node bratley(Tasks &tasks) {
    std::vector<int> all_tasks(tasks.n);
    std::iota(all_tasks.begin(), all_tasks.end(), 0);
    // std::reverse(all_tasks.begin(), all_tasks.end());

    Node initial(0, {}, all_tasks);

    std::stack<Node> stack;
    stack.push(initial);

    int ub = std::numeric_limits<int>::max();
    Node best(0, {}, all_tasks);

    while (!stack.empty()) {
        Node current = stack.top();
        stack.pop();

        if (current.unscheduled.empty()) {
            if (current.c < ub) {
                ub = current.c;
                best = current;
            }
            continue;
        }

        if (current.rule1(tasks)) {
            continue;
        }
        if (current.rule2(tasks, ub)) {
            continue;
        }
        if (current.rule3(tasks)) {
            // clear stack
            stack = std::stack<Node>();
        }

        for (int i = 0; i < current.unscheduled.size(); i++) {
            int schedule_next = current.unscheduled[i];

            int ri = tasks.r[schedule_next];
            int pi = tasks.p[schedule_next];
            int di = tasks.d[schedule_next];

            int new_c = std::max(current.c, ri) + pi;

            if (new_c > di) {
                continue;
            }

            std::vector<int> new_scheduled = current.scheduled;
            new_scheduled.push_back(schedule_next);

            std::vector<int> new_unscheduled;
            for (int n : current.unscheduled) {
                if (n != schedule_next) {
                    new_unscheduled.push_back(n);
                }
            }

            Node next(new_c, new_scheduled, new_unscheduled);
            stack.push(next);
        }
    }

    return best;
}
