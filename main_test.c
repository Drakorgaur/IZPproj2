#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.c"

int main (void) {
    Array a;
    initArray(&a, 3);
//    int **p, N = 2, M = 5; // 2 rows, 5 columns
//    //getNoReturn(&p, N, M);
//    p = get(N, M);
//    fill(p , N, M);
//    print(p, N, M);
//    free2Darray(p ,N);
//    for (int i = 0; i < 8; i++) {
//        insertValue(&a, 'c');
//        printf("%c", a.array[0][i]);
//    }
//    printf("\n");
//    takeNewRow(&a);
//    for (int i = 0; i + 8 < 16; i++) {
//        insertValue(&a, 's');
//        printf("%c", a.array[1][i]);
//    }
//    printf("\n");
//    for (int i = 0; i < 8; i++) {
//        printf("%c", a.array[0][i]);
//    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            insertValue(&a,  's');
            printf("%c", a.array[i][j]);
        }
        if (a.row_used == a.row_size) {
            addRow(&a);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("%d", (int) a.row_used);
    printf("%d", (int) a.column_used);
    freeArray(&a);
    return 0;
}
