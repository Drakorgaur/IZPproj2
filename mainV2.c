#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned short bool;
#define true 1
#define false 0
#define DEFAULT_SIZE 5
#define MAX_SIZE 31
#define forDefault for (int i = 0; i < DEFAULT_SIZE; i++)
#define C memory->Type
#define S memory->Type
#define R memory->Type
#define U memory->Type
#define TYPE memory->Type

typedef struct {
    /*
     * TODO: nesmí obsahovat identifikátory příkazů (viz níže) a klíčová slova true a false + patřit do univerza
     */
    char value;
    char** str;
    int elements_amount;
    int elements_used;
} type;

/*
 * TODO: rename memory
 */
typedef struct {
    type **Type;
    int size;
    int used;
} Memory;

bool valueValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || value == ')'
         || value == '(' ) || (value >= '0' && value <= '9')) return true;
    return false;
}

void var_dump(type *Type) {
    printf("Type: %c\n", Type->value);
    printf("Elements amount: %d\n", Type->elements_amount);
    printf("Elements used: %d\n", Type->elements_used);
    printf("Str: ");
    for (int i = 0; i < Type->elements_used; i++) {
        printf("%s ", Type->str[i]);
    }
    printf("\n");
}


type** initTypeArray() {
    type** Type = malloc(sizeof(type*) * DEFAULT_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i] = malloc(sizeof(type));
    }
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i]->elements_amount = DEFAULT_SIZE;
        Type[i]->elements_used = 0;
        Type[i]->value = '@';
        Type[i]->str = malloc(sizeof(char*) * DEFAULT_SIZE);
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            Type[i]->str[j] = malloc(sizeof(char) * MAX_SIZE);
        }
    }
    return Type;
}

Memory* createMemory() {
    Memory* memory = malloc(sizeof(Memory));
    memory->Type = initTypeArray();
    memory->size = DEFAULT_SIZE;
    memory->used = 1;
    return memory;
}

void addTypeObject(Memory* memory) {
    memory->Type = (type**) realloc(memory->Type, sizeof(type*) * (memory->size + 1));
    if (memory->Type[memory->size] == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
    memory->Type[memory->size]->elements_amount = 0;
    memory->Type[memory->size]->str = malloc(sizeof(char*) * DEFAULT_SIZE);
    for (int j = 0; j < DEFAULT_SIZE; j++) {
        memory->Type[memory->size]->str[j] = malloc(sizeof(char) * MAX_SIZE);
    }
    memory->size++;
}

void resizeMemory(Memory* memory) {
    memory->size++;
    memory->Type = realloc(memory->Type, sizeof(type*) * memory->size);
    for (int i = memory->used; i < memory->size; i++) {
        memory->Type[i] = malloc(sizeof(type));
        memory->Type[i]->elements_amount = memory->size;
        memory->Type[i]->elements_used = 0;
        memory->Type[i]->value = '@';
        memory->Type[i]->str = malloc(sizeof(char*) * memory->size);
        for (int j = 0; j < memory->size; j++) {
            memory->Type[i]->str[j] = malloc(sizeof(char) * MAX_SIZE);
        }
    }
}

void resizeStr(type* T) {
    T->str = realloc(T->str, sizeof(char*) * (T->elements_amount + 1));
    T->str[T->elements_amount] = malloc(sizeof(char) * MAX_SIZE);
    T->elements_amount++;
}

void readFromFile(char* fileName, Memory *memory) {
    FILE *file = fopen(fileName, "r");
    int line = 0;
    int j = 0;
    char* buffer = malloc(sizeof(char) * MAX_SIZE);
    for (int i = 0; !feof(file); i++) {
        char symbol = fgetc(file);
        if (symbol == ' ' || symbol == '\n') {
            j = 0;
            i = 0;
            strcpy(TYPE[line]->str[TYPE[line]->elements_used], buffer);
            strcpy(buffer, "");
            TYPE[line]->elements_used++;
            if (TYPE[line]->elements_used == TYPE[line]->elements_amount) {
                resizeStr(TYPE[line]);
            }
        }
        if (symbol == '\n') {
            if (++memory->used == memory->size) {
                resizeMemory(memory);
            }
            line++;
        }
        if (i == MAX_SIZE) {
            printf("ERROR: element couldn't have size more than 30\n");
            exit(1);
        }
        if (valueValid(symbol)) {
            strncat(buffer, &symbol, 1);
            j++;
        }
    }
    fclose(file);
}

void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->size; i++) {
        for (int j = 0; j < memory->size - 1; j++) {
            free(memory->Type[i]->str[j]);
        }
        free(memory->Type[i]->str);
    }
    free(memory->Type);
    free(memory);
}

int main(int argc, char **argv) {
    Memory* M = createMemory();
    readFromFile(argv[1], M);
//    resizeMemory(M);
    for (int i = 0; i < M->size - 1; i++) {
        var_dump(M->Type[i]);
    }
    freeMemory(M);
    return 0;
}