/* Here we add test function
 * we test.c is only for development, on final dev-step we
 * delete test.c and copy it content to main.c
 * --------------------------------------------------------
 * if you want to crete function or variable that you will
 * use in main.c -> name it with prefix "test_"
 * for example:
 * void test_active() {}
 * --------------------------------------------------------
 * if function print anything in terminal it must have
 * for example:
 * prefixed '\t[test]\t'
 * --------------------------------------------------------
 * if you create function in test.c you have to write
 * comment what function must do & if function can not
 * be understood by other mates - write comments for it steps
 * --------------------------------------------------------
 * be an author of your code - write your name
 */
#include <stdio.h>
typedef unsigned short bool;
#define true 1
#define false 0
/*
 * check if test.c is connected to main.c
 */
void test_active()
{
    printf("\t[test]\ttest.c is enabled");
}
/*
 * prints argc and argv
 */
void getArg(int argc, char** argv)
{
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}
/*
 * print value of 2D array
 */
void show_matrix(int column, int row, char** matrix)
{
    for (int i = 0; i < column; i++) {
        for (int sub_i = 0; sub_i < row; sub_i++) {
            printf("%c", matrix[i][sub_i]);
        }
    }
}

/*
 * returns True
 */
bool True()
{
    return true;
}

/*
 * returns False
 */
bool False()
{
    return false;
}