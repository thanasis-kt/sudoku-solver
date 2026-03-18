#include <csignal>
#include <iostream>
#include <climits>
#include <algorithm>
#include <cmath>
#include <bit>
#include <iterator>
#include <map>
#include <random>
#include <vector>
#include <assert.h>
#include "../include/Sudoku.hpp"

using namespace std;

Sudoku::Sudoku(void) {

}

Sudoku::Sudoku(int** grid,int n) {
    this->n = n;
    this->grid = new int*[n]();
    for (int i = 0; i < n; i++) {
        this->grid[i] = new int[n]();
        for (int j = 0; j < n; j++) {
            this->grid[i][j] = grid[i][j];
        }
    }

    // Create Bitmap and canditates

    // Each element in a bitmap is a binary string of length n, where
    // 1 in the i-th bit means that the corresponding row/collumn/subgrid, can
    // be assigned the value i.
    rows_bitmap = new int[n]();
    cols_bitmap = new int[n]();
    int sq = int(sqrt(n));
    sub_bitmap = new int*[sq]();
    for (int i = 0; i < sq; i++) {
        sub_bitmap[i] = new int[sq]();
        for (int j = 0; j < sq; j++) {
            sub_bitmap[i][j] = (1 << n) - 1;
        }
    }
    for (int i = 0; i < n; i++) {
        rows_bitmap[i] = (1 << n) - 1;
        cols_bitmap[i] = (1 << n) - 1; 
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] > 0) {
                rows_bitmap[i] -= 1 << (grid[i][j] - 1);
                cols_bitmap[j] -= 1 << (grid[i][j] - 1);
                sub_bitmap[i / sq][j / sq] -= 1 << (grid[i][j] - 1);

            }
            else 
                canditates.push_back({i,j});
        }
    }
}

Sudoku::~Sudoku(void) {

}

int Sudoku::get_N(void) {
    return n;
}

int** Sudoku::get_grid(void) {
    return grid;

}

       
ostream& operator<<(ostream& out,Sudoku sudoku) {
    int sq = int(sqrt(sudoku.n));
    auto print_line = [&]() {
            for (int k = 0; k < sudoku.n; k++) {
                if (k % sq == 0 && k > 0)
                    out << "+ ";

                out << "+---";
                            }
            out << "+" << endl;
    };

    auto print_row = [&](int i) {
        out << "|";
        for (int j = 0; j < sudoku.n - 1; j++) {
            if (j % sq == 0 && j > 0)
                out << " |";
            if (sudoku.grid[i][j]  > 0)
                out << " " << sudoku.grid[i][j] << " |";
            else 
                out << "   |";

        }
        if (sudoku.grid[i][sudoku.n - 1] > 0)
            out << " " << sudoku.grid[i][sudoku.n - 1] << " |" << endl;
        else 
            out << "   |" << endl;
    };

    for (int i = 0; i < sudoku.n; i++) {
        if (i % sq == 0 && i > 0) 
            print_line();

        print_line();
        print_row(i);
    }
    print_line();
    return out;
}

Sudoku init_empty_sudoku(int n) {
    int** grid = new int*[n]();
    for (int i = 0; i < n; i++) {
        grid[i] = new int[n]();
    }

    Sudoku sudoku(grid,n);
    delete[] grid;
    return sudoku;
}


pair<int,int> Sudoku::mrv() {
    int sq = int(sqrt(n));
    auto tmp = min_element(canditates.begin(),canditates.end(),
            [&](const pair<int,int> pr1,const pair<int,int> pr2) {
            int x1 = rows_bitmap[pr1.first] & cols_bitmap[pr1.second] & sub_bitmap[pr1.first / sq][pr1.second / sq];
            int x2 = rows_bitmap[pr2.first] & cols_bitmap[pr2.second] & sub_bitmap[pr2.first / sq][pr2.second / sq];
            return popcount(static_cast<unsigned int>(x1)) < popcount(static_cast<unsigned int>(x2));
            
        } );
    if (tmp != canditates.end())
        return *tmp;
    else 
        return {-1,-1};
} 

bool Sudoku::forward_check(pair<int,int> pr,int value) {

    int sq = int(sqrt(n));
    // Lambda function that checks bitmap constraints
    auto check = [&] (int i,int j) {
        // The constraint is false if with the new value assignment, a variable
        // exists without possible values. That means that there exists a neighbouring 
        // variable with bitmap with only the value-th bit raised.
        int mask = rows_bitmap[i] & cols_bitmap[j] & sub_bitmap[i / sq][j / sq];
        int bt =  1 << (value - 1);
        if (grid[i][j] > 0)
            return true;
        if ((mask & bt) > 0) {
            mask -= bt;
            if (mask == 0) {
                return false;
            }    
        }
        return true;
    };

    // We will check for all the neighbours
    // Row check
    for (int i = 0; i < n; i++) {
        if (i == pr.first)
            continue;
        if (!check(i,pr.second))            
            return false;
    }

    // Col Check 
    for (int j = 0; j < n; j++) {
        if (j == pr.second)
            continue;
        if (!check(pr.first,j))
            return false;
    }

    // Subgrid Check
    for (int i = 0; i < sq; i++) {
        for (int j = 0; j < sq; j++) {
            int x = pr.first / sq + i;
            int y = pr.second / sq + j;
            if (x == pr.first && y == pr.second)
                continue;

            if (!check(x,y)) 
                return false;
        }
    }

    return true;
}

void Sudoku::assign(pair<int,int> pr,int value) {
    // We need to update the bitmaps. No need for validity checks, because
    // it is done by forward checking
    int sq = int(sqrt(n));
    int bt = 1 << (value - 1);
    rows_bitmap[pr.first] -= bt;
    cols_bitmap[pr.second] -= bt;
    sub_bitmap[pr.first / sq][pr.second / sq] -= bt;

    // We also need to update our data structures
    grid[pr.first][pr.second] = value; 
    auto x = find(canditates.begin(),canditates.end(),pr);
    assert(x != canditates.end());
    canditates.erase(x);
}

void Sudoku::unassign(pair<int,int> pr) {
    // We need to reverse the impact of assign(pr) in our structure.

    // First we re-update our bitmaps
    int value = grid[pr.first][pr.second];
    int sq = int(sqrt(n));
    int bt = 1 << (value - 1);
    rows_bitmap[pr.first] += bt;
    cols_bitmap[pr.second] += bt;
    sub_bitmap[pr.first / sq][pr.second / sq] += bt;
    // And we also re-insert pr in the candidates list
    grid[pr.first][pr.second] = 0; 
    canditates.push_back(pr);
} 


vector<int> Sudoku::get_values(pair<int,int> pr) {
    int sq = int(sqrt(n));
    vector<int> values;
    int mask = rows_bitmap[pr.first] & cols_bitmap[pr.second] & sub_bitmap[pr.first / sq][pr.second / sq];
    int i = 1;
    while (mask > 0) {
        if ((mask & 1) == 1) 
            values.push_back(i);
        mask /= 2;
        i++;
    }
    assert(values.size() > 0);
    return values;
}

bool Sudoku::solve(int number_of_threads) {
    pair<int,int> cand = mrv();
    if (cand.first != -1 && cand.second != -1) {
        for (auto value: get_values(cand)) {
            if (forward_check(cand, value)) {
                assign(cand,value);
                if (canditates.size() == 0)
                    return true;
                if (!solve(number_of_threads)) {
                    unassign(cand);
                }
                else {
                    return true; 
                }
            }

        }
    }

    // Our assignment is not correct, we need to backtrack
    return false;
}
 
