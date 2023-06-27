#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// 定义数独终局的大小
const int SIZE = 9;

// 定义数独终局的结构体
struct Sudoku {
    int grid[SIZE][SIZE];
};

// 检查给定数字是否在指定的行中重复
bool isNumberInRow(const Sudoku& sudoku, int row, int number) {
    for (int col = 0; col < SIZE; ++col) {
        if (sudoku.grid[row][col] == number) {
            return true;
        }
    }
    return false;
}

// 检查给定数字是否在指定的列中重复
bool isNumberInColumn(const Sudoku& sudoku, int col, int number) {
    for (int row = 0; row < SIZE; ++row) {
        if (sudoku.grid[row][col] == number) {
            return true;
        }
    }
    return false;
}

// 检查给定数字是否在指定的九宫格中重复
bool isNumberInBox(const Sudoku& sudoku, int boxStartRow, int boxStartCol, int number) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (sudoku.grid[row + boxStartRow][col + boxStartCol] == number) {
                return true;
            }
        }
    }
    return false;
}

// 检查给定位置是否可以放置指定的数字
bool isSafe(const Sudoku& sudoku, int row, int col, int number) {
    return !isNumberInRow(sudoku, row, number) &&
           !isNumberInColumn(sudoku, col, number) &&
           !isNumberInBox(sudoku, row - row % 3, col - col % 3, number);
}

// 填充数独终局
bool fillSudoku(Sudoku& sudoku) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (sudoku.grid[row][col] == 0) {
                // 尝试填充数字
                for (int number = 1; number <= SIZE; ++number) {
                    if (isSafe(sudoku, row, col, number)) {
                        sudoku.grid[row][col] = number;
                        if (fillSudoku(sudoku)) {
                            return true;
                        }
                        sudoku.grid[row][col] = 0;  // 回溯
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 生成一个随机的数独终局
Sudoku generateRandomSudoku() {
    Sudoku sudoku;
    fill(sudoku.grid[0], sudoku.grid[0] + SIZE * SIZE, 0);  // 初始化数独终局为全0

    // 随机填充数字
    random_device rd;
    mt19937 gen(rd());
    vector<int> numbers(SIZE);
    for (int i = 1; i <= SIZE; ++i) {
        numbers[i - 1] = i;
    }
    shuffle(numbers.begin(), numbers.end(), gen);

    for (int i = 0; i < SIZE; ++i) {
        sudoku.grid[0][i] = numbers[i];
    }

    fillSudoku(sudoku);  // 填充数独终局

    return sudoku;
}

// 解答数独
bool solveSudoku(Sudoku& sudoku) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (sudoku.grid[row][col] == 0) {
                for (int number = 1; number <= SIZE; ++number) {
                    if (isSafe(sudoku, row, col, number)) {
                        sudoku.grid[row][col] = number;
                        if (solveSudoku(sudoku)) {
                            return true;
                        }
                        sudoku.grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 从文件中加载数独终局
bool loadSudokuFromFile(const string& filename, Sudoku& sudoku) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "Failed to open input file." << endl;
        return false;
    }

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (!(inputFile >> sudoku.grid[row][col])) {
                cout << "Invalid input file format." << endl;
                return false;
            }
        }
    }

    inputFile.close();
    return true;
}

// 保存数独终局到文件
bool saveSudokuToFile(const string& filename, const Sudoku& sudoku) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cout << "Failed to open output file." << endl;
        return false;
    }

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            outputFile << sudoku.grid[row][col] << " ";
        }
        outputFile << endl;
    }

    outputFile.close();
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "用法: ./sudoku_generator <-c/-s> <文件名>" << endl;
        return 1;
    }

    string option = argv[1];
    string filename = argv[2];

    if (option == "-c") {
        int count = stoi(filename);

        if (count <= 0) {
            cout << "无效的数量: " << count << endl;
            return 1;
        }

        ofstream outputFile("数独终局.txt");
        if (!outputFile) {
            cout << "无法打开输出文件。" << endl;
            return 1;
        }

        for (int i = 0; i < count; ++i) {
            Sudoku sudoku = generateRandomSudoku();
            for (int row = 0; row < SIZE; ++row) {
                for (int col = 0; col < SIZE; ++col) {
                    outputFile << sudoku.grid[row][col] << " ";
                }
                outputFile << endl;
            }
            outputFile << endl;
        }

        outputFile.close();
        cout << "数独题目生成成功。" << endl;
    } else if (option == "-s") {
        Sudoku sudoku;
        if (!loadSudokuFromFile(filename, sudoku)) {
            return 1;
        }

        if (solveSudoku(sudoku)) {
            if (saveSudokuToFile("game_solve.txt", sudoku)) {
                cout << "数独解答成功。解答保存在 game_solve.txt 中。" << endl;
            } else {
                cout << "无法保存解答的数独。" << endl;
                return 1;
            }
        } else {
            cout << "无法解答数独。" << endl;
            return 1;
        }
    } else {
        cout << "无效的选项: " << option << endl;
        return 1;
    }

    return 0;
}

