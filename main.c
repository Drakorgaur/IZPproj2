#include <stdio.h>
#include "test.c"
/*
 * commented because defined in test.c
 * when we go production - we uncomment & delete test.c
 */
//typedef unsigned short bool;
//#define true 1
//#define false 0

void operateWithFile(FILE *file, int* columns, int* rows, bool create, char** matrix)
{
    int column = 1;
    int row = 1;
    while (!feof(file))
    {
        if (fgetc(file) != '\n') {
            if (create) {
                matrix[column][row] = (char)fgetc(file);
            }
            row++;
        } else {
            column++;
            if (row > *rows) {
                *rows = row;
            }
            row = 0;
        }
    }
    *columns = column;
}
void createFileMatrix()
{
    /*
     * init i, rows, columns to create matrix
     */
    int i = 1, rows = 0, columns = 1;
    char **empty = NULL;
    FILE *file = stdin;
    operateWithFile(file, &columns, &rows, false, (char **) empty);
    char matrix[columns][rows];
    operateWithFile(file, &columns, &rows, true, (char **) matrix);
    show_matrix(columns, rows, (char **) matrix);
}
int main(int argc, char** argv) {
    /*
     * func to print argc and argv[]
     */
    getArg(argc, argv);

    test_active();
    createFileMatrix();

    return 0;
}
