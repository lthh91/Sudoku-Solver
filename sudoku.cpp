#include <iostream>
#include <string>
#include <fstream>
#include <tuple>

#define empty 0
#define N 9
#define none -1

using namespace std;

class SudokuSolver {
    private:
        int grid[N][N];
        int firstEmptyRow = none;
        int firstEmptyCol = none;
        bool isCellEmpty(int row, int col) {
            if (grid[row][col] == empty) {
                return true;
            }
            return false;
        }

        bool isNumberUsed(int number, int rowStart, int rowEnd, int colStart, int colEnd) {
            for (int row = rowStart; row < rowEnd; row++) {
                for (int col = colStart; col < colEnd; col++) {
                    if (grid[row][col] == number) return true;
                }
            }
            return false;
        }

        bool usedInCol(int number, int col) {
            return isNumberUsed(number, 0, N, col, col+1);
        }

        bool usedInRow(int number, int row) {
            return isNumberUsed(number, row, row+1, 0, N);
        }

        bool usedInBox(int number, int row, int col) {
            int rowStart = (row / 3) * 3;
            int rowEnd = rowStart + 3;
            int colStart = (col / 3) * 3;
            int colEnd = colStart + 3;
            return isNumberUsed(number, rowStart, rowEnd, colStart, colEnd);
        }

        bool numberCanBeUsed(int number, int row, int col) {
            return !usedInCol(number, col) && !usedInRow(number, row) && !usedInBox(number, row, col);
        }

        std::tuple<int, int> findEmptyCell() {
            for (int row = 0; row < N; row++) {
                for (int col = 0; col < N; col++) {
                    if (isCellEmpty(row, col)) return std::make_tuple(row, col);
                }
            }
            return std::make_tuple(none, none);
        }

        bool isSolved(int &row, int &col) {
            for (row = 0; row < N; row++) {
                for (col = 0; col < N; col++) {
                    if (isCellEmpty(row, col)) return false;
                }
            }
            return true;
        }

    public:
        void setGrid(int gridToSet[N][N]) {
            for (int row = 0; row < N; row++) {
                for (int col = 0; col < N; col++) {
                    grid[row][col] = gridToSet[row][col];
                }
            }
        }

        void readSudoku(std::string fileName) {
            ifstream sudokuFile(fileName);
            char currentChar;
            int row = 0;
            int col = 0;
            while (sudokuFile) {
                currentChar = sudokuFile.get();
                if (currentChar == ' ') continue;
                if (currentChar == '\n') {
                    col = 0;
                    row++;
                    continue;
                }
                grid[row][col] = currentChar - '0';
                col++;
            }
        }

        bool solve() {
            int row, col;
            std::tie(row, col) = findEmptyCell();
            if (row == none && col == none) return true;
            if (row == firstEmptyRow && col == firstEmptyCol) return false;
            if (firstEmptyRow == none) firstEmptyRow = row;
            if (firstEmptyCol == none) firstEmptyCol = col;
            for (int number = 1; number <= N; number++) {
                if (numberCanBeUsed(number, row, col)) {
                    grid[row][col] = number;
                    if (solve()) return true;
                }
            }
            grid[row][col] = empty;
            return false;
        }

        void printSudoku() {
            for (int row = 0; row < N; row++) {
                for (int col = 0; col < N; col++)
                    cout<< grid[row][col]<<"  ";
                cout<<endl;
            }
        }
};

int main(int argc, char* argv[])
{
    SudokuSolver solver = SudokuSolver();
    string fileName = "easy.sudoku";
    if (argc > 1) fileName = argv[1];
    solver.readSudoku(fileName);
    if (solver.solve()) {
        solver.printSudoku();
    } else {
        cout << "The sudoku can't be solved" << endl;
    }
}
