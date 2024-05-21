#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

#define BOX 9

// Function to print the Sudoku grid
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

// Function to check if it's safe to place 'num' at grid[row][col]
bool isSafe(char grid[BOX][BOX], int row, int col, char num) {
    // Check row
    for (int x = 0; x < BOX; x++)
        if (grid[row][x] == num)
            return false;

    // Check column
    for (int x = 0; x < BOX; x++)
        if (grid[x][col] == num)
            return false;

    // Check 3x3 box
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

// Function to fill the Sudoku grid
bool fillGrid(char grid[BOX][BOX], int row, int col) {
    if (row == BOX - 1 && col == BOX)
        return true;

    if (col == BOX) {
        row++;
        col = 0;
    }

    // Shuffle numbers for random selection
    vector<char> numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    random_device rd;
    mt19937 g(rd());
    shuffle(numbers.begin(), numbers.end(), g);

    for (char num : numbers) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (fillGrid(grid, row, col + 1))
                return true;

            grid[row][col] = '.';
        }
    }
    return false;
}

// Function to generate a complete Sudoku grid
void generateCompleteGrid(char grid[BOX][BOX]) {
    srand(time(0)); // Initialize random number generator

    for (int i = 0; i < BOX; i++)
        for (int j = 0; j < BOX; j++)
            grid[i][j] = '.';

    fillGrid(grid, 0, 0);
}

// Function to solve the Sudoku grid
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

            grid[row][col] = '.';
        }
    }
    return false;
}

// Function to count the number of solutions for the Sudoku grid
int countSolutions(char grid[BOX][BOX], int row, int col) {
    if (row == BOX - 1 && col == BOX)
        return 1;

    if (col == BOX) {
        row++;
        col = 0;
    }

    if (grid[row][col] != '.')
        return countSolutions(grid, row, col + 1);

    int count = 0;
    for (char num = '1'; num <= '9'; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            count += countSolutions(grid, row, col + 1);
            if (count > 1) {
                grid[row][col] = '.';
                return count;
            }

            grid[row][col] = '.';
        }
    }
    return count;
}

// Function to remove 'num_holes' digits from the Sudoku grid
void removeDigits(char grid[BOX][BOX], int num_holes) {
    int count = num_holes;
    while (count > 0) {
        int cellId = rand() % (BOX * BOX);
        int row = cellId / BOX;
        int col = cellId % BOX;
        if (grid[row][col] != '.') {
            char backup = grid[row][col];
            grid[row][col] = '.';

            char tempGrid[BOX][BOX];
            copy(&grid[0][0], &grid[0][0] + BOX * BOX, &tempGrid[0][0]);

            if (countSolutions(tempGrid, 0, 0) != 1) {
                grid[row][col] = backup; // Restore if it does not have a unique solution
            } else {
                count--;
            }
        }
    }
}

// Function for user input and interactive Sudoku solving
void userInput(char grid[BOX][BOX]) {
    string input;
    int row, col;
    char num;
    while (true) {
        cout << "Saisir 'soluce' pour voir la solution ou entrer le numero de la ligne (0-8), de la colonne (0-8) et le nombre (1-9) a placer:"     << endl;
        cin >> input;

        if (input == "soluce") break;

        try {
            row = stoi(input);
            cin >> col >> num;
        } catch (...) {
            cout << "Saisie invalide." << endl;
            continue;
        }

        if (row >= 0 && row < BOX && col >= 0 && col < BOX && num >= '1' && num <= '9') {
            if (grid[row][col] == '.' && isSafe(grid, row, col, num)) {
                grid[row][col] = num;
                cout << "Nombre placer correctement." << endl;
            } else {
                cout << "Nombre invalide ou position deja occuper." << endl;
            }
        } else {
            cout << "Saisie invalide" << endl;
        }
        // Print grid after each input
        print(grid); 
    }
}

int main() {
    char grid[BOX][BOX];

    // Choose difficulty level
    int difficulty;
    cout << "Choose a difficulty level:" << endl;
    cout << "1. Facile" << endl;
    cout << "2. Moyen" << endl;
    cout << "3. Difficile" << endl;
    cout << "Votre choix: ";
    cin >> difficulty;

    int num_holes;
    switch (difficulty) {
        case 1: 
            // Easy
            num_holes = 30;
            break;
        case 2:
            // Medium
            num_holes = 40;
            break;
        case 3:
            // Hard
            num_holes = 50; 
            break;
        default:
            cout << "Choix invalide, par default ça sera le niveau moyen." << endl;
            num_holes = 40;
            break;
    }

    generateCompleteGrid(grid);
    // Adjust according to selected difficulty
    removeDigits(grid, num_holes); 

    cout << "Generation de la grille:" << endl;
    print(grid);

    userInput(grid);

    if (solveSudoku(grid, 0, 0)) {
        cout << "La solution:" << endl;
        print(grid);
    } else {
        cout << "Aucune solution disponible." << endl;
    }

    return 0;
}
