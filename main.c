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

bool valueValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || value == ' ' || value == ')'
    || value == '(' )) return true;
        return false;

}

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
    matrix = (char**) malloc(sizeof (char *) * (*row + 1));
    for (int i = 0; i < *row; i++) {
        matrix[i] = (char*) malloc(sizeof (char) * (*column + 1));
    }
    for (int i = 0; i < *row; i++) {
        for (int j = 0; j < *column; j++) {
            printf(" %d[%d] = ", i, j);
            char tmp;
            tmp = fgetc(file);
            if (tmp == '\n') {
                break;
            }
            if (charValid(tmp)) {
                matrix[i][j] = tmp;
                printf("%c", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    FILE* file = fopen(argv[1], "r");
    char **matrix = NULL;
    int column = 1;
    int row = 1;
    getArraySize(file, &row, &column);
    fclose(file);
    file = fopen(argv[1], "r");
    fillArray(file, matrix, &row, &column);

    free(matrix);

    fclose(file);
    printf("\n\n%d\n", row);
    printf("%d", column);
}
