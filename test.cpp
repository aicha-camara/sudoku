#include <iostream>

using namespace std;

#define BOX 9

void print(char arr[BOX][BOX]) {
    for (int i = 0; i < BOX; i++) {
        for (int j = 0; j < BOX; j++) {
            if (j % 3 == 0 && j != 0)
                cout << " | ";
            cout << arr[i][j] << " ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0 && i != BOX - 1)
            cout << "---------------------" << endl;
    }
}

bool isSafe(char grid[BOX][BOX], int row, int col, char num) {
    for (int x = 0; x < BOX; x++)
        if (grid[row][x] == num)
            return false;

    for (int x = 0; x < BOX; x++)
        if (grid[x][col] == num)
            return false;

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool solveSudoku(char grid[BOX][BOX], int row, int col) {
    if (row == BOX - 1 && col == BOX)
        return true;

    if (col == BOX) {
        row++;
        col = 0;
    }

    if (grid[row][col] != '.')
        return solveSudoku(grid, row, col + 1);

    for (char num = '1'; num <= '9'; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (solveSudoku(grid, row, col + 1))
                return true;
        }

        grid[row][col] = '.';
    }
    return false;
}

void userInput(char grid[BOX][BOX]) {
    int row, col;
    char num;
    while (true) {
        print(grid);
        cout << "Saisir numero de la ligne (0-8), de la colonne (0-8) et le nombre (1-9) to place (ou -1 pour la soluce):";
        cin >> row;
        if (row == -1) break;
        cin >> col >> num;

        if (row >= 0 && row < BOX && col >= 0 && col < BOX && num >= '1' && num <= '9') {
            if (grid[row][col] == '.' && isSafe(grid, row, col, num)) {
                grid[row][col] = num;
                cout << "Nombre bien placer." << endl;
            } else {
                cout << "Mauvais nombre" << endl;
            }
        } else {
            cout << "Saisis invalide" << endl;
        }
    }
}

int main() {
    char grid[BOX][BOX] = { { '3', '.', '6', '5', '.', '8', '4', '.', '.' },
                            { '5', '2', '.', '.', '.', '.', '.', '.', '.' },
                            { '.', '8', '7', '.', '.', '.', '.', '3', '1' },
                            { '.', '.', '3', '.', '1', '.', '.', '8', '.' },
                            { '9', '.', '.', '8', '6', '3', '.', '.', '5' },
                            { '.', '5', '.', '.', '9', '.', '6', '.', '.' },
                            { '1', '3', '.', '.', '.', '.', '2', '5', '.' },
                            { '.', '.', '.', '.', '.', '.', '.', '7', '4' },
                            { '.', '.', '5', '2', '.', '6', '3', '.', '.' } };

    userInput(grid);

    if (solveSudoku(grid, 0, 0)) {
        cout << "Solution Sudoku:" << endl;
        print(grid);
    } else {
        cout << "Solution indisponible." << endl;
    }

    return 0;
}
