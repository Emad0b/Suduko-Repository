#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define G 9

void printSudoku(int sudoku[G][G]) {
    for (int i= 0; i <G; i++) {
        printf("|");
        for (int j= 0; j <G; j++) {
            printf("%2d", sudoku[i][j]);
            if ((j+1)%3== 0&&j <G-1) {
                printf("|"); // Separate columns within subgrids
            }
        }
        printf("|\n");
        if ((i+1)% 3== 0&&i <G-1) {
            printf("+------+------+------+\n"); // Separate rows between subgrids
        }
    }
}


void shuffle(int array[G]) {
    for (int i= G-1; i >0; i--) {
        int j= rand()% (i+1);
        int temp= array[i];
        array[i]= array[j];
        array[j]= temp;
    }
}

void generateSudoku(int sudoku[G][G]) {
    int base[G]= {1,2,3,4,5,6,7,8,9};

    //random number generator
    srand(time(NULL));
    // Shuffle the base array
    shuffle(base);

    // Fill the first row of the Sudoku grid
    for (int i= 0; i <G; i++) {
        sudoku[0][i]= base[i];
    }

    // Generate Sudoku puzzle by permuting the first row
    for (int i= 1; i <G; i++) {
        for (int j= 0; j <G; j++) {
            sudoku[i][j]= sudoku[0][(j + i)%G];
        }
    }
}

int main() {
    int sudoku[G][G];

    clock_t startTime= clock();
    generateSudoku(sudoku);
    clock_t endTime= clock();

    double totalTime= (double)(endTime-startTime)/ CLOCKS_PER_SEC;
    printSudoku(sudoku);

    printf("Time taken: %f seconds\n",totalTime);
    return 0;
}
