#include <iostream>
#include <iterator>
#include "../include/Sudoku.hpp"

int main(int argc,char* argv[]) {
    int n = atoi(argv[1]);
    Sudoku sudoku = init_empty_sudoku(n);
    cout << sudoku << endl;
    int valid = sudoku.solve(1);
    if (valid)
        cout << sudoku << endl;
    else 
        cout << "No Solution" << endl;
    return 0;
}
