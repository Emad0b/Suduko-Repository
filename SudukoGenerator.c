//
// Created by Emad on 09/11/2023.
//
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <time.h>
#include "sys/time.h"
#include "pthread.h"
#include <math.h>
#include "ctype.h"

struct cacheI {
    int row, col, box;
};
int size = 9, box_size = 3, max = 9, boxes = 3, cell_count = 81, max_index = 80;

double total = 0;
int placeholder = 0;
int *num_arr;
bool **row_hash, **col_hash, **box_hash;
struct cacheI *indices;
struct timeval timer;
double start;
bool running = true;

void generate(int index, int *board);
int isDigit(char string[]);
double microtime();
void parseArgs(int argc, char *argv[]);
void init();
void *printResult();

void init() {
    num_arr = calloc(size, sizeof(int));
    for (int i = 0; i < max; ++i) {
        num_arr[i - 1] = i;
    }
    row_hash = calloc(size, sizeof(bool *));
    col_hash = calloc(size, sizeof(bool *));
    for (int i = 0; i < size; ++i) {
        row_hash[i] = calloc(max, sizeof(bool));
        col_hash[i] = calloc(max, sizeof(bool));
    }
    box_hash = calloc(boxes * boxes, sizeof(bool *));
    for (int i = 0; i < boxes * boxes; ++i) {
        box_hash[i] = calloc(max, sizeof(bool));
    }
    indices = calloc(cell_count, sizeof(struct cacheI));
    for (int i = 0; i < cell_count; ++i) {
        int row = i / size;
        int col = i % size;
        indices[i].row = row;
        indices[i].col = col;
        indices[i].box = row / box_size * boxes + col / box_size;
    }
}

void *printResult() {
    while (running) {
        double elapsed = (microtime() - start) * 1000;
        double snapshot = total;
        printf("Total Boards Created: %.0f\t\n", snapshot);
        printf("Total Time: %.0fms\t\n", elapsed);
        printf("Time per Board: %fms\t\n", elapsed / snapshot);
        printf("Boards per Second: %.1f\t\n", (snapshot / elapsed) * 1000);
        printf("\033[4A"); // ANSI escape sequence to move the cursor up 4 lines
        printf("\r");     // Carriage return to overwrite the previous lines
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {

    parseArgs(argc, argv);

    register int *board = calloc(cell_count, sizeof(int));
    for (int i = 0; i < cell_count; ++i) {
        board[i] = placeholder;
    }

    init();

    pthread_t t1;
    pthread_create(&t1, NULL, printResult, NULL);

    start = microtime();
    generate(0, board);

    running = false;
    // Wait for the statistics thread to finish
    pthread_join(t1, NULL);

    // Print final statistics
    double elapsed = (microtime() - start) * 1000;
    printf("Total Boards Created: %.0f\n", total);
    printf("Total Time: %.0fms\n", elapsed);
    printf("Time per Board: %fms\n", elapsed / total);
    printf("Boards per Second: %.0f\n", total / elapsed * 1000);

    // Free allocated memory
    free(board);
    return 0;
}

void generate(int index, int *board) {
    struct cacheI *cache = &indices[index];

    if (index== max_index) {
        for (int i = 0; i < max; ++i) {
            if (row_hash[cache->row][i] || col_hash[cache->col][i] || box_hash[cache->box][i]) {
                continue;
            }

            board[index] = num_arr[i];
            total++;
        }
    } else {
        for (int i = 0; i < max; ++i) {
            if (row_hash[cache->row][i] || col_hash[cache->col][i] || box_hash[cache->box][i]) {
                continue;
            }

            board[index] = num_arr[i];
            row_hash[cache->row][i] = true;
            col_hash[cache->col][i] = true;
            box_hash[cache->box][i] = true;
            generate(index + 1, board);
            row_hash[cache->row][i] = false;
            col_hash[cache->col][i] = false;
            box_hash[cache->box][i] = false;
        }
    }

    board[index] = placeholder;
}

int isDigit(char string[]) {
    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] != '0' && !isdigit(string[i])) {
            return 0;
        }
    }
    return 1;
}

double microtime() {
    gettimeofday(&timer, NULL);
    return (double)timer.tv_sec + ((double)timer.tv_usec / 1000000.0);
}

void parseArgs(int argc, char *argv[]) {

    if (argc > 4) {
        printf("Too many arguments provided. Only one allowed.\n");
        exit(1);
    }

    if (argc >= 2) {
        if (isDigit(argv[1]) && atoi(argv[1]) > 0 && atoi(argv[1]) <= 100) {
            size = max = atoi(argv[1]);
            cell_count = size * size;
            max_index = cell_count - 1;
            box_size = (int)sqrt(size);
            boxes = size / box_size;
        } else {
            printf("Invalid size format. A positive integer not exceeding 100 should be provided.\n");
            exit(1);
        }
    }

    if (argc >= 3) {
        if (isDigit(argv[2]) && atoi(argv[2]) > 0) {
            box_size = atoi(argv[2]);
            boxes = size / box_size;
        } else {
            printf("Invalid box_size format. A positive integer should be provided.\n");
            exit(1);
        }
    }

    if (argc >= 4) {
        if (isDigit(argv[3]) && atoi(argv[3]) > 0) {
            max = atoi(argv[3]);
        } else {
            printf("Invalid max_value format. A positive integer should be provided.\n");
            exit(1);
        }
    }

    if (size % box_size != 0) {
        printf("Invalid box_size value. The size should be divisible by the box_size.\n");
        exit(1);
    }

    if (max < size || max < box_size * box_size) {
        printf("Invalid max_value format. The max should be more than or equal to the size and the square of the box_size.\n");
        exit(1);
    }
}
