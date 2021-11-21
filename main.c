#include <stdio.h>

int main(int argc, char** argv) {
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    int i = 0;
    int rows = 1;
    int columns = 1;
    while (!feof(stdin))
    {
        if (fgetc(stdin) != '\n') {
            i++;
        } else {
            columns++;
            if (i > rows) {
                rows = i;
            }
            i = 0;
        }
    }
    printf("\n\ncolumns = %d\nrows = %d\n", columns, rows);
    int matrix[columns][rows];
    int value = 0;
    for (int column = 0; column < columns; column++) {
        for (int row = 0; row < rows; row++) {
            matrix[column][row] = value;
            value++;
            printf("%d\t", matrix[column][row]);
        }
        printf("\n");
    }
    return 0;
}
