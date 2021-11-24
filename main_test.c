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
//        a->column_used = a->row_used = 0;
        a->column_used = a->row_used = a->column_size = a->row_size = initialSize;
    }
}

void upgrade(Array *A)
{
    A->array = (char **)realloc(A->array, sizeof(char **)*A->row_size);

    for(int i=0; i<A->row_used; ++i){
        A->array[i] = (char *)realloc(A->array[i], sizeof(char)*A->column_size);
    }

    for(size_t i=A->row_used; i<A->row_size; ++i){
        *(A->array+i) = (char *)malloc(sizeof(char)*A->column_size);
    }

}

int main(int argc, char const *argv[]) {

    int N = 10;
    Array A;

    initArray(&A, N);

    for(int i=0; i<A.row_size; ++i){
        for(int j=0; j<A.row_size; ++j){
            A.array[i][j] = 'a';
        }
    }

    for(int i=0; i<A.row_size; ++i){
        printf("%d", i);
        for(int j=0; j<A.row_size; ++j){
            printf("%c ", A.array[i][j]);
        }
        printf("\n");
    }
    A.row_size += 3;
    A.column_size += 1;

    upgrade(&A);

    for(int i=0; i<A.row_size; ++i){
        printf("%d", i);
        for(int j=0; j<A.column_size; ++j){
            A.array[i][j] = 'c';
            printf("%c ", A.array[i][j]);
        }
        printf("\n");
    }

//    for(size_t i=0; i<A.row_used; ++i){
//        for(size_t j=A.row_used; j<A.row_size; ++j){
//            A.array[i][j] = 'b';
//        }
//    }
//    for(size_t i=A.row_used; i<A.row_size; ++i){
//        for(size_t j=0; j<A.row_size; ++j){
//            A.array[i][j] = 'c';
//        }
//    }
//
//    for(size_t i=0; i<A.row_size; ++i){
//        for(size_t j=0; j<A.row_size; ++j){
//            printf("%c ", A.array[i][j]);
//        }
//        printf("\n");
//    }

    return 0;
}