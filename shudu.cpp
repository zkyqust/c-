#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// ���������վֵĴ�С
const int SIZE = 9;

// ���������վֵĽṹ��
struct Sudoku {
    int grid[SIZE][SIZE];
};

// �����������Ƿ���ָ���������ظ�
bool isNumberInRow(const Sudoku& sudoku, int row, int number) {
    for (int col = 0; col < SIZE; ++col) {
        if (sudoku.grid[row][col] == number) {
            return true;
        }
    }
    return false;
}

// �����������Ƿ���ָ���������ظ�
bool isNumberInColumn(const Sudoku& sudoku, int col, int number) {
    for (int row = 0; row < SIZE; ++row) {
        if (sudoku.grid[row][col] == number) {
            return true;
        }
    }
    return false;
}

// �����������Ƿ���ָ���ľŹ������ظ�
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

// ������λ���Ƿ���Է���ָ��������
bool isSafe(const Sudoku& sudoku, int row, int col, int number) {
    return !isNumberInRow(sudoku, row, number) &&
           !isNumberInColumn(sudoku, col, number) &&
           !isNumberInBox(sudoku, row - row % 3, col - col % 3, number);
}

// ��������վ�
bool fillSudoku(Sudoku& sudoku) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (sudoku.grid[row][col] == 0) {
                // �����������
                for (int number = 1; number <= SIZE; ++number) {
                    if (isSafe(sudoku, row, col, number)) {
                        sudoku.grid[row][col] = number;
                        if (fillSudoku(sudoku)) {
                            return true;
                        }
                        sudoku.grid[row][col] = 0;  // ����
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// ����һ������������վ�
Sudoku generateRandomSudoku() {
    Sudoku sudoku;
    fill(sudoku.grid[0], sudoku.grid[0] + SIZE * SIZE, 0);  // ��ʼ�������վ�Ϊȫ0

    // ����������
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

    fillSudoku(sudoku);  // ��������վ�

    return sudoku;
}

// �������
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

// ���ļ��м��������վ�
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

// ���������վֵ��ļ�
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
        cout << "�÷�: ./sudoku_generator <-c/-s> <�ļ���>" << endl;
        return 1;
    }

    string option = argv[1];
    string filename = argv[2];

    if (option == "-c") {
        int count = stoi(filename);

        if (count <= 0) {
            cout << "��Ч������: " << count << endl;
            return 1;
        }

        ofstream outputFile("�����վ�.txt");
        if (!outputFile) {
            cout << "�޷�������ļ���" << endl;
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
        cout << "������Ŀ���ɳɹ���" << endl;
    } else if (option == "-s") {
        Sudoku sudoku;
        if (!loadSudokuFromFile(filename, sudoku)) {
            return 1;
        }

        if (solveSudoku(sudoku)) {
            if (saveSudokuToFile("game_solve.txt", sudoku)) {
                cout << "�������ɹ�����𱣴��� game_solve.txt �С�" << endl;
            } else {
                cout << "�޷��������������" << endl;
                return 1;
            }
        } else {
            cout << "�޷����������" << endl;
            return 1;
        }
    } else {
        cout << "��Ч��ѡ��: " << option << endl;
        return 1;
    }

    return 0;
}

