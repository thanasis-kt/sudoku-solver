# C++ Optimized Sudoku Solver

Sudoku solver written in c++, modeled as a Constraint Satisfaction Problem (CSP).
 The problem also is modeled as a generic sudoku, offering solutions for 16x16 and 25x25 also.

Our CSP approach uses backtracking, equiped with the following heuristics:
    - Forward Checking (FC)
    - Minimum Remaining Values (MRV)
    - Bitwise Optimizations: each cell in the grid is represented as a N lengthed
    sequence, where the i-th bit is 1 when that value can be assigned


