#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.c"

int main (void) {
    int i = 1000;
    Array a;
    initArray(&a, 8);
//    a.array[0][0] = 's';
//    printf("%c", a.array[0][0]);
    for (int j = 0; j < i; j++) {
        if (!(j % 50)) {
            takeNewRow(&a);
        }
        insertValueToArrayRow(&a, 's');
    }
    for (int q = 0; q < (int) a.row_used; q++) {
        for (int sub_i = 0; q < (int) a.column_used; sub_i++) {
            printf("%c ", a.array[q][sub_i]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("%d", (int) a.row_used);
    printf("%d", (int) a.column_used);
    printf("%c", a.array[5][2]);
    freeArray(&a);
    return 0;
}
