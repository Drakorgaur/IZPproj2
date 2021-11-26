#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned short bool;
#define true 1
#define false 0
#define DEFAULT_SIZE 2
#define MAX_SIZE 31
#define forDefault for (int j = 0; j < DEFAULT_SIZE; j++)
#define foreachType for (int i = 0; i < Type->elements_used; i++)
#define TYPE memory->Type

typedef struct {
    /*
     * TODO: nesmí obsahovat identifikátory příkazů (viz níže) a klíčová slova true a false + patřit do univerza
     */
    char value;
    char* row; //99999
    int row_length;
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

typedef struct {
    int* lines;
    char functions[10][10];
} set;

bool valueValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9'))) return true;
    return false;
}

void var_dump(type *Type) {
    printf("Type: %c\n", Type->value);
    printf("Row: %s\n", Type->row);
    printf("Elements amount: %d\n", Type->elements_amount);
    printf("Elements used: %d\n", Type->elements_used);
    printf("Str: ");
    foreachType {
        printf("%s ", Type->str[i]);
    }
    printf("\n\n");
}

type** initTypeArray() {
    type** Type = malloc(sizeof(type*) * DEFAULT_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i] = malloc(sizeof(type));
    }
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i]->elements_amount = DEFAULT_SIZE;
        Type[i]->elements_used = 0;
        Type[i]->value = 'U';
        Type[i]->row = malloc(sizeof(char) * DEFAULT_SIZE);
        Type[i]->row_length = DEFAULT_SIZE;
        Type[i]->str = malloc(sizeof(char*) * DEFAULT_SIZE);
        forDefault Type[i]->str[j] = malloc(sizeof(char) * MAX_SIZE);
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

void resizeMemory(Memory* memory) {
    memory->size++;
    memory->Type = realloc(memory->Type, sizeof(type*) * memory->size);
    memory->Type[memory->used] = malloc(sizeof(type));
    memory->Type[memory->used]->elements_amount = DEFAULT_SIZE;
    memory->Type[memory->used]->elements_used = 0;
    memory->Type[memory->used]->value = 'U';
    memory->Type[memory->used]->row = malloc(sizeof(char) * DEFAULT_SIZE);
    memory->Type[memory->used]->row_length = DEFAULT_SIZE;
    memory->Type[memory->used]->str = (char **)malloc(sizeof(char *) * DEFAULT_SIZE);
    forDefault memory->Type[memory->used]->str[j] = (char *)malloc(sizeof(char) * MAX_SIZE);
}

void resizeStr(type* T) {
    T->str = realloc(T->str, sizeof(char*) * (T->elements_amount + 1));
    T->str[T->elements_amount] = malloc(sizeof(char) * MAX_SIZE);
    T->elements_amount++;
}

void resizeRow(type* T) {
    T->row = realloc(T->row, sizeof(char) * (T->row_length + 1));
    T->row_length++;
}

void readFromFile(char* fileName, Memory *memory) {
    FILE *file = fopen(fileName, "r");
    int line = 0, j = 0;
    bool newLineChecker = false;
    char* buffer = malloc(sizeof(char) * MAX_SIZE);
    for (int i = 0, row = 1; !feof(file); i++) {
        char symbol = fgetc(file);
        if (symbol == ' ' || symbol == '\n' || symbol == 'U') {
            i = 0;
            if (j == 0) {
                if (newLineChecker && symbol == '\n') {
                    row++;
                    continue;
                }
                newLineChecker = false;
                if (symbol == 'U') {
                    TYPE[line]->value = symbol;
                    continue;
                }
                TYPE[line]->value = buffer[0];
                if (row > ((10 ^ TYPE[line]->row_length) - 1)) {
                    resizeRow(TYPE[line]);
                    TYPE[line]->row_length++;
                }
                sprintf(TYPE[line]->row, "%d", row);
                strcpy(buffer, "");
                j++;
            } else {
                strcpy(TYPE[line]->str[TYPE[line]->elements_used], buffer);
                strcpy(buffer, "");
                TYPE[line]->elements_used++;
                if (TYPE[line]->elements_used == TYPE[line]->elements_amount) resizeStr(TYPE[line]);
            }
        }
        if (symbol == '\n') {
            j = 0; row++;
            newLineChecker = true;
            if (++memory->used == memory->size) resizeMemory(memory);
            line++;
        }
        if (i == MAX_SIZE) {
            printf("ERROR: element couldn't have size more than 30\n");
            exit(1);
        }
        if (valueValid(symbol)) strncat(buffer, &symbol, 1);
    }
    fclose(file);
}

void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->size; i++) {
        for (int j = 0; j < memory->Type[i]->elements_amount; j++) free(memory->Type[i]->str[j]);
        free(memory->Type[i]->str);
        free(memory->Type[i]->row);
    }
    free(memory->Type);
    free(memory);
}


int main(int argc, char **argv) {
    Memory *M = createMemory();
    readFromFile(argv[1], M);
    for (int i = 0; i < M->used; i++) var_dump(M->Type[i]);

    char setFunc[10][10] = {"empty", "card", "complement", "union", "intersect", "minus", "subseteq", "subset",
                            "equals"};

    freeMemory(M);
    return 0;
}

