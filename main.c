#include <stdio.h>
#include "test.c"
/*
 * commented because defined in test.c
 * when we go production - we uncomment & delete test.c
 */
//typedef unsigned short bool;
//#define true 1
//#define false 0
#include <stdio.h>
#include <stdlib.h>

void getArraySize(FILE* file, int *row, int *column) {
    int columns = 0, *p = &columns;
    while (!feof(file))
    {
        if (fgetc(file) != '\n') {
            (*column)++;
        } else {
            (*row)++;
            if (*column > *p) {
                *p = *column;
            }
            *column = 0;
        }
    }
    *column = *p;
}

void fillArray(FILE* file, char** matrix, int *row, int *column) {
    matrix = (char**) malloc(sizeof (char *) * *row);
    for (int i = 0; i < *column; i++) {
        matrix[i] = (char*) malloc(sizeof (char) * *column);
    }
//    int i = 0, j;
//    while (!feof(file)) {
//        char tmp;
//        fscanf(file, "%c\n", &tmp);
//        for (j = 0; j < *column; j++) {
//            if (tmp == '\n'){
//                i++;
//                break;
//            }
//            matrix[i][j] = tmp;
//        }
//    }
    for (int i = 0; i < *row; i++) {
        for (int j = 0; j < *column; j++) {
            char tmp;
            tmp = fgetc(file);
            if (tmp == '\n'){
                printf("\n");
                break;
            }
            matrix[i][j] = tmp;
            printf("%c", matrix[i][j]);
        }
    }

}

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    FILE* file = fopen(argv[1], "r");
    char **matrix;
    int column = 1;
    int row = 1;
    getArraySize(file, &row, &column);
    fclose(file);
//    file = fopen(argv[1], "r");
//    getArraySize(file, &row, &column);
    file = fopen(argv[1], "r");
    fillArray(file, matrix, &row, &column);
    for (int i = 0; i < row; i++) {
        free(matrix[i]);
    }
    free(matrix);
    fclose(file);
    printf("\n\n%d\n", row);
    printf("%d", column);
}
