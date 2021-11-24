#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned short bool;
#define true 1
#define false 0

typedef struct {
    char **array;
    int column_size;
    int column_used;
    int row_size;
    int row_used;
} Array;

typedef struct {
   char* str;
   int size;
} universum;

typedef struct {
    char* str;
} commands;

typedef struct {
    char* relation_1;
    char* relation_2;
} relation;

bool valueValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || value == ' ' || value == ')'
         || value == '(' ) || (value >= '0' && value <= '9')) return true;
    return false;
}

bool checkLenght(char *str, int size) {
    if (strlen(str) == size) return true;
    return false;
}

bool compare(char* a, char* b)
{
    while (*a != '\0' || *b != '\0') {
        if (*a == *b) {
            a++;
            b++;
        }
        else if ((*a == '\0' && *b != '\0')
                 || (*a != '\0' && *b == '\0')
                 || *a != *b) {
            return false;
        }
    }
    return true;
}

void initArray(Array *a, int initialSize) {
    a->array = (char **)malloc(initialSize * sizeof *(a->array));
    if (a->array) {
        for (int i = 0; i < initialSize; i++)
            a->array[i] = (char*)malloc(initialSize * sizeof*(a->array[i]));
        a->column_used = a->row_used = 0;
        a->column_size = a->row_size = initialSize;
    }
}

void upgrade(Array *A)
{
    A->array = (char **)realloc(A->array, sizeof(char **)*(A->row_size));

    for(int i=0; i<A->row_size-1; ++i){
        A->array[i] = (char *)realloc(A->array[i], sizeof(char)*A->column_size);
    }

    *(A->array+A->row_size-1) = (char *)malloc(sizeof(char)*A->column_size);
    /*
     * replace rubish to ' '
     */
    for (int i = 0; i < A->row_size; i++) {
        A->array[i][A->column_size-1] = ' ';
    }
}

void var_dump(Array* a) {
    for (int r = 0; r < a->row_size; r++) {
        printf("%d| ", r);
        for (int c = 0; c < a->column_size; c++) {
            printf("%c", a->array[r][c]);
        }
        printf("\n");
    }
}

void clearSpace(Array *A) {
    for (int i = A->column_used; i < A->column_size; i++) {
        A->array[A->row_used][i] = ' ';
    }
}

void fillArray(FILE* file, Array* A) {
    while (!feof(file)) {
        for (int  c = 0; !feof(file) ; ++c) {
            char ch = fgetc(file);
            if (ch == '\n') {
                clearSpace(A);
                if (A->row_used + 1 == A->row_size) {
                    A->row_size++;
                    upgrade(A);
                }
                A->row_used++;
                A->column_used = 0;
                break;
            }
            if (A->column_used + 1 == A->column_size) {
                A->column_size++;
                upgrade(A);
            }
            if (valueValid(ch)) {
                A->array[A->row_used][A->column_used] = ch;
                A->column_used++;
            } else {
                A->array[A->row_used][A->column_used] = '\0';
            }
        }
    }
    clearSpace(A);
    A->column_used--;
}

bool checkLine(char* str, int size, Array* A) {
    printf("\n[checkLine]\n");
    int element_size = 0;
    for (int line = 2; line < A->column_size; line++) {
        if (str[line] == ' ') {
            element_size = 0;
            continue;
        }
        if (element_size++ > size) {
            printf("element more than 30");
            exit(1);
        }
    }


    return true;
}

bool validateArray(Array* A) {
    int size = 30;
    char temp[] = {'U', 'S', 'R', 'C', '\0'};
    for (int row = 0; row < A->row_size; row++) {
        bool exist = false, *pexist = &exist;
        for (int ch = 0; ch < strlen(temp); ch++){
//            printf("%c and %c\n", temp[ch], A->array[row][0]);
            if (A->array[row][0] == temp[ch] && A->array[row][1] == ' ') {
                *pexist = true;
                break;
            }
        }
        if (!exist) {
            return false;
        }
    }
    for (int row = 0; row < A->row_size; row++) {
        checkLine(A->array[row], size, A);
    }
    return true;
}

void freeArray(Array *a) {
    for (int i = 0; i < a->row_size; i++){
        free(a->array[i]);
    }
    free(a->array);
    a->array = NULL;
    a->column_size = a->column_used = a->row_size = a->row_used = 0;
}

int main(int argc, char const *argv[]) {
    Array A;
    initArray(&A, 4);
    FILE* file;
    file = fopen(argv[1], "r");
    fillArray(file, &A);
    var_dump(&A);
    if (!(validateArray(&A))){
        freeArray(&A);
        return 1;
    }
    freeArray(&A);
    return 0;
}