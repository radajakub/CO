import numpy as np
# import gurobipy as g

import sys


class Problem:
    def __init__(self, d: np.ndarray, e: np.ndarray, D: int) -> None:
        assert len(d) == len(e)
        assert len(d) == 24
        assert D >= 0

        self.d = d
        self.e = e
        self.D = D

    def __str__(self) -> str:
        res = f'd: {self.d}\n'
        res += f'e: {self.e}\n'
        res += f'D: {self.D}\n'
        return res


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


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise ValueError("You must provide an input path and output path")

    in_file = sys.argv[1]
    out_file = sys.argv[2]

    problem = load(in_file)

    # solve the problem with gurobi

    save(np.zeros(24, dtype=int), 0, out_file)
