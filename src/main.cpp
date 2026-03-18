#include <iostream>
#include <iterator>
#include "../include/Sudoku.hpp"

int main(int argc,char* argv[]) {
    int n = atoi(argv[1]);
    int** grid = new int*[n]();
    for (int i = 0; i < n; i++) {
        grid[i] = new int[n]();
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    Sudoku sudoku(grid,n);
    cout << sudoku << endl;
    int valid = sudoku.solve(1);
    if (valid)
        cout << sudoku << endl;
    else 
        cout << "No Solution" << endl;

    for (int i = 0; i < n; i++) {
        delete[] grid[i];
    }
    delete[] grid;
    return 0;
}
