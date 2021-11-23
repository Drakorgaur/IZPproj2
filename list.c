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
    a->array = (char **)malloc(initialSize * sizeof *(a->array));
    if (a->array) {
        for (int i = 0; i < initialSize; i++)
            a->array[i] = (char*)malloc(initialSize * sizeof*(a->array[i]));
        a->column_used = a->row_used = 0;
        a->column_size = a->row_size = --initialSize;
    }
}

void addRow(Array *a)
{
    a->row_size++;
    char **tmp = realloc( a->array, sizeof *a->array * (a->row_size));
    if (tmp)
    {
        a->array = tmp;
        a->array[a->row_size] = malloc( sizeof *a->array[a->row_size] * a->column_size );
    }
    a->row_used++;
}

void reallocRowArray(Array *a)
{
    a->column_size++;
    for (size_t i = 0; i < a->row_size; i++ )
    {
        char *tmp = realloc( a->array[i], sizeof *a->array[i] * a->column_size);
        if (tmp)
        {
            a->array[i] = tmp;
        }
    }
}

void insertValue(Array *a, char value)
{
    a->array[a->row_used][a->column_used] = value;
    if (++a->column_used == a->column_size) {
        reallocRowArray(a);
    }
}


//void mallocArray(Array *a, size_t column_size, size_t row_size) {
//    size_t N = a->column_size;
//
//    if (column_size == a->column_size) {
//        /*
//         * realloc 1d space for array
//         */
//        a->array = (char **)realloc(a->array, ++a->column_size * sizeof(char **));
//    }
//
//    if (row_size == a->row_size) {
//        /*
//         * realloc old-init values
//         */
//
//        for(int i=0; i < N; ++i){
//            a->array[i] = realloc(a->array[i], sizeof(char) * a->row_size);
//        }
//
//        /*
//         * malloc new space for values
//         */
//        for(int i=(int) N; i < a->column_size; ++i){
//            a->array[i] = (char *)malloc(sizeof(char) * ++(a->row_size));
//        }
//    }
//}
//
//void insertValue(Array *a, char value)
//{
//    a->array[a->column_used][a->row_used] = value;
//
//    if (++(a->row_used) == a->row_size) {                 // here we increment row value
//        mallocArray(a, a->column_used, a->row_used);    // update Array row by 1
//    }
//}
//
//void takeNewRow(Array *a)
//{
//    if (++(a->column_used) == a->column_size) {           // here we increment column value
//        mallocArray(a, a->column_used, a->row_used);    // update Array column by 1
//    }
//    a->row_used = 0;
//}

void freeArray(Array *a) {
//    for (int i = 0; i < a->column_size; i++){
//        free(a->array[i]);
//    }
    free(a->array);
    a->array = NULL;
    a->column_size = a->column_used = a->row_size = a->row_used = 0;
}

