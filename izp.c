#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int *items;
    int size;
} vector_t;

void vector_construct(vector_t* v, int size) {
    v->items = (int*)malloc(sizeof (int ) * size);
    v->size = size;
}
void vector_destuctor(vector_t* v) {
    free(v->items);
    v->size = 0;
}
void vector_add(vector_t* v, int item) {
    v->items = realloc(v->items, sizeof (int) * (v->size + 1));
    if (v->items == NULL) {
        free(v->items);
        return;
    }
    v->items[v->size] = item;
    v->size++;
}

char *find_substring(char* str, char* substr)
{
    for(int i = 0; i < strlen(str); i++)
    {
        if (*(str+i) == substr[0]) {
            for (int j = 0; j < strlen(substr); j++) {
                if (*(str+i+j) != *(substr+j)) {
                    break;
                }
                if (j + 1 == strlen(substr)) {
                    return (str+i);
                }
            }
        }
    }
    return NULL;
}

void replace(char* str, char* old, char* new_subst ) {
    int size = 0;
    char *p = find_substring(str, old);
    if (p == NULL) {
        return;
    }
    if (strlen(old) != strlen(new_subst)){
        size_t temp = strlen(str);
        size = (int) strlen(new_subst) - (int) strlen(old);
        str = (char *)realloc(str, sizeof (char ) * (temp + size));
        if (str == NULL) {
            return;
        }
    }
    for(int i = 0; i < strlen(str); i++)
    {
        if (*(str+i) == old[0]) {
            for (int j = 0; j < strlen(old); j++) {
                *(str+i+j) = *(new_subst+j);
            }
        }
    }
    while (*str) {
        printf("%c", *str);
        *str++;
    }
}
int main(int argc, char** argv)
{
    char *str;
    char *substr;
//    while (*str) {
//        printf("%c", *p);
//        *p++;
//    }
    substr = malloc(sizeof (char) * strlen("world"));
    str = (char *)malloc(sizeof (char) * strlen("Hello, world"));
    str = "hello, world";
    substr = "world";
    int i = 0;
    char *p = find_substring(str, substr);


    replace(str, substr, "winte");


    return 0;
}