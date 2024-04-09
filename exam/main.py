#!/usr/bin/env python3

import sys
import numpy as np
import gurobipy as g


class Grid:
    def __init__(self, path: str):
        with open(path, 'r') as f:
            self.COLS, self.ROWS, self.T, self.L, self.P = map(int, f.readline().split())
            self.c = np.zeros((self.ROWS, self.COLS), dtype=int)
            for r in range(self.ROWS):
                self.c[r, :] = list(map(int, f.readline().split()))
            self.p = np.zeros(self.P, dtype=int)
            for i in range(self.P):
                self.p[i] = int(f.readline().strip())

    def print(self):
        print(f'{self.COLS} {self.ROWS} {self.T} {self.L} {self.P}')
        print(self.c)
        print(self.p)


def solve(grid: Grid):
    m = g.Model('grid')

    # positions of built wind turbines
    xy = m.addVars(grid.ROWS, grid.COLS, vtype=g.GRB.BINARY, name='xy')
    y = m.addVars(grid.COLS, vtype=g.GRB.BINARY, name='y')
    b = m.addVars(grid.ROWS, grid.COLS, vtype=g.GRB.BINARY, name='built')

    # maximize total output considering the loss
    m.setObjective(g.quicksum(grid.c[r, c] * xy[r, c] - grid.L * b[r, c] for r in range(grid.ROWS) for c in range(grid.COLS)), sense=g.GRB.MAXIMIZE)

    # build at most T turbines
    m.addConstr(xy.sum() <= grid.T)

    # add loss
    for r in range(grid.ROWS):
        m.addConstr(b[r, 0] == 0)
        for c in range(1, grid.COLS):
            # xy[r, c] and xy[r, c-1] => b[r, c]
            m.addConstr(1 + b[r, c] >= xy[r, c - 1] + xy[r, c])

    # in specified columns P, build either 3 or none turbines
    for p in grid.p:
        col_sum = g.quicksum(xy[r, p] for r in range(grid.ROWS))
        m.addConstr(col_sum == y[p] * 3)

    m.optimize()

    val = int(m.getObjective().getValue())

    placements = [(c, r) for r in range(grid.ROWS) for c in range(grid.COLS) if xy[r, c].X > 0.5]

    return val, placements


def save(path: str, value: int, placements: list[tuple[int, int]]):
    with open(path, "w") as f:
        f.write(f'{value}\n')
        for x, y in placements:
            f.write(f'{x} {y}\n')


if __name__ == '__main__':
    if len(sys.argv) < 3:
        raise Exception("Usage: python main.py <input_path> <output_path>")

    grid = Grid(sys.argv[1])
    # grid.print()

    val, placements = solve(grid)
    save(sys.argv[2], val, placements)
