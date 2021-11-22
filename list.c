/*
 * This file is created to support main.c with dynamic 2D-arrays
 */
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char **array;
    size_t column_size;
    size_t column_used;
    size_t row_size;
    size_t row_used;
} Array;

void initArray(Array *a, size_t initialSize) {
    a->array = (char **)malloc(initialSize * sizeof(char **));
    for (int i = 0; i < initialSize; i++)
        a->array[i] = (char*)malloc(initialSize * sizeof(char));
    a->column_used = a->row_used = 0;
    a->column_size = a->row_size = initialSize;
}

void mallocArray(Array *a) {
    size_t N = a->column_used;
    size_t M = N * 2;
    a->array = (char **)realloc(a->array, sizeof(char **) * (M));

    for(int i=0; i<N; ++i){
        *(a->array + i) = (char *)realloc(*(a->array + i), sizeof(char) * M);
    }

    for(int i=(int) N; i<M; ++i){
        *(a->array + i) = (char *)malloc(sizeof(char) * M);
    }
    a->row_used = 0;
}

void takeNewRow(Array *a)
{
    a->column_used++;
}

void insertValueToArrayRow(Array *a, char element)
{
    if (a->row_used == a->row_size) {
        mallocArray(a);
    }
    a->array[a->column_used][a->row_used] = element;
    printf("%c ", a->array[a->column_used][a->row_used]);
    a->row_used++;
}

void freeArray(Array *a) {
    for (int i = 0; i < a->column_size; i++)
        free(a->array[i]);
    free(a->array);
    a->array = NULL;
    a->column_size = a->column_used = a->row_size = a->row_used = 0;
}

