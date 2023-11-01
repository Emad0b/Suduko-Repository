#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define G 9

void printSudoku(int sudoku[G][G],int user[G][G]) {
    for (int i = 0; i <G; i++) {
        printf("|");
        for (int j = 0; j < G; j++) {
            if (user[i][j] == 0) {
                printf("  ");
            } else {
                printf("%2d", user[i][j]);
            }
            if ((j + 1) % 3 == 0 && j < G - 1) {
                printf("|"); 
            }
        }
        printf("|\n");
        if ((i + 1) % 3 == 0 && i < G - 1) {
            printf("+------+------+------+\n"); // Separate rows between subgrids
        }
    }
}

void shuffle(int array[G]) {
    for (int i = G - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void generateSudoku(int sudoku[G][G], int user[G][G]) {
    int base[G] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    srand(time(NULL));
    // Shuffle the base array
    shuffle(base);

    // Fill the first row of the Sudoku grid
    for (int i = 0; i < G; i++) {
        sudoku[0][i] = base[i];
        user[0][i] = base[i];
    }

    // Generate Sudoku puzzle by permuting the first row
    for (int i = 1; i < G; i++) {
        for (int j = 0; j < G; j++) {
            sudoku[i][j] = sudoku[0][(j + i) % G];
            user[i][j] = 0; // Initialize user's Sudoku with empty cells
        }
    }
}

int isSolved(int sudoku[G][G], int user[G][G]) {
    for (int i = 0; i < G; i++) {
        for (int j = 0; j < G; j++) {
            if (user[i][j] != sudoku[i][j]) {
                return 0; // Puzzle is not solved
            }
        }
    }
    return 1; // Puzzle is solved
}

int main() {
    int sudoku[G][G];
    int user[G][G];

    clock_t startTime = clock();
    generateSudoku(sudoku, user);
    clock_t endTime = clock();

    double totalTime = (double)(endTime - startTime)/ CLOCKS_PER_SEC;

    printf("Sudoku Puzzle:\n");
    printSudoku(sudoku, user);

    while (!isSolved(sudoku, user)) {
        int row, col, num;
        printf("Enter row (1-9), column (1-9), and number (1-9) to fill in a cell (e.g., 3 5 7): ");
        scanf("%d %d %d", &row, &col, &num);

        // Check if the user's input is valid
        if (row >= 1 && row <= G && col >= 1 && col <= G && num >= 1 && num <= G) {
            if (user[row - 1][col - 1] == 0) {
                user[row - 1][col - 1] = num;
            } else {
                printf("Cell is already filled.\n");
            }
        } else {
            printf("Invalid input. Please enter valid row, column, and number (1-9).\n");
        }

        printf("Updated Sudoku:\n");
        printSudoku(sudoku, user);
    }

    printf("Congratulations! You solved the Sudoku puzzle.\n");
    printf("Time taken: %f seconds\n", totalTime);

    return 0;
}
