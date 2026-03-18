#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Sudoku {
    private:
        int n;
        int** grid;
        int* rows_bitmap; // rows bitmap
        int* cols_bitmap; // columns bitmap
        int** sub_bitmap; // Subgrids bitmap

        vector<pair<int,int>> canditates;

        /* Variables to test our algorithms performance */
        int states_visited; // Number of different states visited
        int constraints_checked; // Number of constraint checks 
        int backjumps; // Number of backjumps


    public:
        Sudoku(void);
        Sudoku(int** grid,int n);
        ~Sudoku(void);

        // Accessors
        int get_N(void);
        int** get_grid(void);

        bool solve(int number_of_threads);

         // Checks whether or not this assignment satisfies current constraints,
         // by checking it's neighbors value set. 
        bool forward_check(pair<int,int> pr,int value);

        // Uses Minimum Remaining Values heuristic to return a canditate
        pair<int,int> mrv(); 
                                                        
        // Applies changes after a assignment. Doesn't check for constraint satisfaction.
        // You should use forward_check before calling
        void assign(pair<int,int> pr,int value); 
        // Unasigns the value of pr cell from the current assignment. 
        void unassign(pair<int,int> pr); 

        vector<int> get_values(pair<int,int> pr);

        
        friend ostream& operator<<(ostream& out,Sudoku sudoku);
};

Sudoku init_empty_sudoku(int n);
