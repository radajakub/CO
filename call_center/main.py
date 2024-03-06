#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import gurobipy as g

import sys


class Problem:
    def __init__(self, d: np.ndarray, e: np.ndarray, D: int) -> None:
        assert len(d) == len(e)
        assert len(d) == 24
        assert D >= 0

        # demands for weekdays
        self.d = np.hstack([d] * 5)
        self.d = np.hstack([self.d, e, e])
        # maximum difference in demand and employees
        self.D = D
        # length of every shift
        self.L = 8
        # number of hours in a day
        self.n = len(self.d)

    def __str__(self) -> str:
        res = f'd: {self.d}\n'
        res += f'e: {self.e}\n'
        res += f'D: {self.D}\n'
        return res

    def shift_range(self, i: int) -> range:
        return range(i - (self.L - 1), i + 1)

    def plot(self, starts: np.ndarray) -> None:
        num_shifts = [sum([starts[k % self.n] for k in self.shift_range(i)]) for i in range(self.n)]
        margin = 0.2
        width = 0.3
        plt.figure(figsize=(8, 4))
        plt.bar([h + margin for h in range(self.n)], self.d, width=width, color='green')
        plt.bar([h + margin + width for h in range(self.n)], num_shifts, width=width, color='yellow')
        plt.xlabel("hour")
        plt.legend(['demand', 'number shifts'], ncol=2, bbox_to_anchor=(0.8, 1.1))
        plt.xlim(0, self.n)
        plt.ylim(0, max(num_shifts + self.d) + 1)
        plt.xticks(range(self.n), [i % self.n for i in range(self.n)])
        plt.grid()
        plt.show()


def load(path: str) -> Problem:
    with open(path, 'r') as file:
        d = np.array([int(x) for x in file.readline().strip().split()], dtype=int)
        e = np.array([int(x) for x in file.readline().strip().split()], dtype=int)
        D = int(file.readline().strip())

        return Problem(d=d, e=e, D=D)


def save(xs: np.ndarray, val: int, path: str) -> None:
    with open(path, 'w') as file:
        file.write(f'{val}\n')
        file.write(' '.join([str(x) for x in xs]))


def solve(p: Problem) -> tuple[np.ndarray, int]:
    model = g.Model('Call Center')

    # start times of shifts
    x = model.addVars(p.n, lb=0, ub=g.GRB.INFINITY, vtype=g.GRB.INTEGER, name='x')
    # slack variable to express absolute value in the objective
    z = model.addVars(p.n, lb=0, ub=g.GRB.INFINITY, vtype=g.GRB.INTEGER, name='x')

    for i in range(p.n):
        # absolute value constraints
        model.addConstr(p.d[i] - g.quicksum(x[j % p.n] for j in p.shift_range(i)) <= z[i], 'abs1')
        model.addConstr(g.quicksum(x[j % p.n] for j in p.shift_range(i)) - p.d[i] <= z[i], 'abs2')
        model.addConstr(z[i] >= 0, 'abs3')
        # constrain difference between demand and employees
        model.addConstr(p.d[i] - g.quicksum(x[j % p.n] for j in p.shift_range(i)) <= p.D, 'demand')

    # minimize number of started shifts
    model.setObjective(g.quicksum(z), sense=g.GRB.MINIMIZE)

    model.optimize()

    return np.array([x[i].x for i in range(p.n)], dtype=int), int(model.objVal)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise ValueError("You must provide an input path and output path")

    in_file = sys.argv[1]
    out_file = sys.argv[2]

    problem = load(in_file)

    # solve the problem with gurobi
    xs, val = solve(problem)

    # problem.plot(xs)

    save(xs, val, out_file)
