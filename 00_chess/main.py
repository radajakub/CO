#!/usr/bin/env python3

import sys
import gurobipy as g

BOARD_SIZE = 8


class Chess:
    @staticmethod
    def coords_to_chess(r: int, c: int) -> str:
        return f'{chr(c + ord("a"))}{r + 1}'

    @staticmethod
    def chess_to_coords(chess: str) -> tuple[int, int]:
        return ord(chess[0]) - ord('a'), int(chess[1]) - 1

    def __init__(self, n_rooks: int, rook_positions: list[tuple[int, int]]):
        self.n_rooks = n_rooks
        self.rook_positions = rook_positions

    def get_neighbors(self, r: int, c: int) -> list[tuple[int, int]]:
        neighbors = []
        for dr, dc in [(1, 2), (2, 1), (-1, 2), (-2, 1), (1, -2), (2, -1), (-1, -2), (-2, -1)]:
            if 0 <= r + dr < BOARD_SIZE and 0 <= c + dc < BOARD_SIZE:
                neighbors.append((r + dr, c + dc))
        return neighbors


def load_problem(input_path: str) -> Chess:
    with open(input_path, 'r') as f:
        n_rooks = int(f.readline())
        rook_positions = []
        for _ in range(n_rooks):
            rook_positions.append(Chess.chess_to_coords(f.readline().strip()))

    return Chess(n_rooks, rook_positions)


def solve_model(chess: Chess) -> list[tuple[int, int]]:
    m = g.Model('chess')

    # x indicates positions where to put the knights
    x = m.addVars(BOARD_SIZE, BOARD_SIZE, vtype=g.GRB.BINARY, name='x')
    # helper variables for big M
    y = m.addVars(BOARD_SIZE, BOARD_SIZE, vtype=g.GRB.BINARY, name='y')

    # maximimze number of placed knights
    m.setObjective(g.quicksum(x[r, c] for r in range(BOARD_SIZE) for c in range(BOARD_SIZE)), g.GRB.MAXIMIZE)

    M = 9
    # x[r, c] == 1 => sum(x[i, j]) == 0 for all (i, j) in neighbors
    # knights constraints
    for r in range(BOARD_SIZE):
        for c in range(BOARD_SIZE):
            # either x[r, c] == 0 or sum(x[i, j]) <= 0 for all (i, j) in neighbors
            m.addConstr(g.quicksum(x[i, j] for i, j in chess.get_neighbors(r, c)) <= y[r, c] * M)
            m.addConstr(x[r, c] <= (1 - y[r, c]) * M)

    # rook constraints
    for r, c in chess.rook_positions:
        m.addConstr(g.quicksum(x[r, i] for i in range(BOARD_SIZE)) == 0)
        m.addConstr(g.quicksum(x[i, c] for i in range(BOARD_SIZE)) == 0)

    m.optimize()

    return [(r, c) for r in range(BOARD_SIZE) for c in range(BOARD_SIZE) if x[r, c].X > 0.5]


def show_neighbors(chess: Chess, r: int, c: int):
    neighbors = chess.get_neighbors(r, c)
    for i in range(BOARD_SIZE):
        for j in range(BOARD_SIZE):
            if i == r and j == c:
                print('K', end=' ')
            elif (i, j) in neighbors:
                print('X', end=' ')
            else:
                print('.', end=' ')
        print()


def show(rooks: list[tuple[int, int]], knights: list[tuple[int, int]]):
    for r in range(BOARD_SIZE):
        for c in range(BOARD_SIZE):
            if (r, c) in rooks:
                print('R', end=' ')
            elif (r, c) in knights:
                print('K', end=' ')
            else:
                print('.', end=' ')
        print()


def save(output_path: str, knights: list[tuple[int, int]]):
    list_coords = sorted([Chess.coords_to_chess(c, r) for r, c in knights])
    with open(output_path, 'w') as f:
        f.write(f'{len(knights)}\n')
        for i in range(len(knights)):
            f.write(f'{list_coords[i]}\n')


if __name__ == '__main__':
    if len(sys.argv) < 3:
        raise Exception("Usage: python main.py <input_path> <output_path>")

    chess = load_problem(sys.argv[1])

    knights = solve_model(chess)

    save(sys.argv[2], knights)

    # print(len(knights))

    # show(chess.rook_positions, knights)
