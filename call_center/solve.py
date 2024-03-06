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
        dline = file.readline().strip()
        d = np.array([int(x) for x in dline.split()])

        eline = file.readline().strip()
        e = np.array([int(x) for x in eline.split()])

        D = int(file.readline().strip())

        return Problem(d=d, e=e, D=D)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise ValueError("You must provide an input path and output path")

    in_file = sys.argv[1]
    out_file = sys.argv[2]

    problem = load(in_file)
