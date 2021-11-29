#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned short bool;
#define true 1
#define false 0
#define DEFAULT_SIZE 2
#define MAX_SIZE 31
#define forDefault for (int j = 0; j < DEFAULT_SIZE; j++)
#define foreachElementInType for (int i = 0; i < Type->elements_used; i++)
#define foreachResult for (int i = 0; i < commands->used; i++)
#define SELECT_FROM_TYPES for (int i = 0; i < memory->used; i++) {
#define WHERE for (int j = 0; j < TYPE[i]->elements_used; j++) {
#define VALUE(a) if (TYPE[i]->header == (a)) {
#define ROW(a) if (strcmp(TYPE[i]->row, (a)) == 0) {
#define end }}}
#define TYPE memory->Type
#define EMPTY 'E'

typedef struct {
    /*
     * TODO: nesmí obsahovat identifikátory příkazů (viz níže) a klíčová slova true a false + patřit do univerza
     */
    char header;
    char* row;
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
    char** array;
    int size;
    int used;
} result;

bool headerValid(char header) {
    if (header == 'U' || header == 'R' || header == 'C' || header == 'S') return true;
    return false;
}

bool valueValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9'))) return true;
    return false;
}

void var_dump(type *Type) {
    printf("Type: %c\n", Type->header);
    printf("Row: %s\n", Type->row);
    printf("Elements amount: %d\n", Type->elements_amount);
    printf("Elements used: %d\n", Type->elements_used);
    printf("Str: ");
    foreachElementInType {
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
        Type[i]->header = EMPTY;
        Type[i]->row = malloc(sizeof(char) * DEFAULT_SIZE);
        Type[i]->row_length = DEFAULT_SIZE + 1;
        Type[i]->str = malloc(sizeof(char*) * DEFAULT_SIZE);
        forDefault Type[i]->str[j] = malloc(sizeof(char) * MAX_SIZE);
    }
    return Type;
}

Memory* createMemory() {
    Memory* memory = malloc(sizeof(Memory));
    memory->Type = initTypeArray();
    memory->size = DEFAULT_SIZE;
    memory->used = 0;
    return memory;
}

void resizeMemory(Memory* memory) {
    memory->size++;
    memory->Type = realloc(memory->Type, sizeof(type*) * memory->size);
    memory->Type[memory->used] = malloc(sizeof(type));
    memory->Type[memory->used]->elements_amount = DEFAULT_SIZE;
    memory->Type[memory->used]->elements_used = 0;
    memory->Type[memory->used]->header = EMPTY;
    memory->Type[memory->used]->row = malloc(sizeof(char) * DEFAULT_SIZE);
    memory->Type[memory->used]->row_length = DEFAULT_SIZE + 1;
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
/*
 * TODO: test for
 *      1) 30 symbols
 *      2) rows more than 9999
 */

/*
 * TODO: create exit(1) for
 *      1) No newline at EOF
 */
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
                    TYPE[line]->header = symbol;
                    continue;
                }
                TYPE[line]->header = valueValid(buffer[0]) ? buffer[0] : 'U';
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
            fclose(file);
            printf("ERROR: element couldn't have size more than 30\n");
            exit(1);
        }
        if (valueValid(symbol)) strncat(buffer, &symbol, 1);
    }
    fclose(file);
}


void readFromFileV2(char* filename, Memory* memory) {
    FILE* file = fopen(filename, "r");
    int row  = 1;
    bool headerChecker = true;
    char symbol, *wordBuffer;
    wordBuffer = malloc(sizeof(char) * MAX_SIZE);
    strcpy(wordBuffer, "");
    memory->used = -1;
    for (int wordSize = 0; !feof(file); wordSize++) {
        symbol = fgetc(file);
        while (symbol == ' ' || symbol == '\n') {
            if (symbol == ' ') {
                if (headerChecker) {
                    symbol = fgetc(file);
                    headerChecker = false;
                    continue;
                }

                strcpy(TYPE[memory->used]->str[TYPE[memory->used]->elements_used], wordBuffer);

                if (strcmp(wordBuffer, "") != 0) {
                    TYPE[memory->used]->elements_used++;
                }
                strcpy(wordBuffer, "");
                if (TYPE[memory->used]->elements_used == TYPE[memory->used]->elements_amount) resizeStr(TYPE[memory->used]);
                wordSize = 0;
            }
            if (symbol == '\n') {
                row++;
                wordSize = 0;
                symbol = fgetc(file);

                if (!headerChecker) {
                    strcpy(TYPE[memory->used]->str[TYPE[memory->used]->elements_used], wordBuffer);
                    if (strcmp(wordBuffer, "") != 0) {
                        TYPE[memory->used]->elements_used++;
                    }
                    strcpy(wordBuffer, "");
                    if (TYPE[memory->used]->elements_used == TYPE[memory->used]->elements_amount) resizeStr(TYPE[memory->used]);
                }
                headerChecker = true;
                continue;
            }
            symbol = fgetc(file);
        }
        if (wordSize == MAX_SIZE) {
            fclose(file);
            free(wordBuffer);
            printf("ERROR: element couldn't have size more than 30\n");
            exit(1);
        }
        if (headerChecker) {
            if (headerValid(symbol)) {
                if (++memory->used == memory->size) resizeMemory(memory);
                TYPE[memory->used]->header = symbol;
                TYPE[memory->used]->row = malloc(sizeof(char) * 3);
                sprintf(TYPE[memory->used]->row, "%d", row);
                continue;
            } else {
                if(feof(file)) {
                    continue;
                }
                free(wordBuffer);
                fclose(file);
                exit(1);
            }
        }
        if (valueValid(symbol)) strncat(wordBuffer, &symbol, 1);
        headerChecker = false;
    }
    memory->used++;
    fclose(file); free(wordBuffer);
}


void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->used; i++) {
        for (int j = 0; j < memory->Type[i]->elements_amount; j++) free(memory->Type[i]->str[j]);
        free(memory->Type[i]->str);
        free(memory->Type[i]->row);
    }
    free(memory->Type);
    free(memory);
}

//create function to free() type
void freeType(type* Type) {
    for (int i = 0; i < Type->elements_amount; i++) free(Type->str[i]);
    free(Type->str);
    free(Type->row);
    free(Type);
}

void freeResult(result* res) {
    for (int i = 0; i < res->used; i++) free(res->array[i]);
    free(res->array);
    free(res);
}

result* createResult() {
    result* res = malloc(sizeof(result));
    res->size = DEFAULT_SIZE;
    res->used = 0;
    res->array = malloc(sizeof(char*) * DEFAULT_SIZE);
    forDefault res->array[j] = malloc(sizeof(char) * MAX_SIZE);
    return res;
}

void resizeResult(result* res) {
    res->size++;
    res->array = realloc(res->array, sizeof(char*) * res->size);
    res->array[res->used] = malloc(sizeof(char) * MAX_SIZE);
}

void selectByValue(Memory* memory, result* res, char ch) {
    SELECT_FROM_TYPES WHERE VALUE(ch)
                strcpy(res->array[res->used], TYPE[i]->row);
                if (++res->used == res->size) resizeResult(res);
                break;
    end
}

//create function to copy type and return it
void copyType(type* newType, type* oldType) {
    newType->header = oldType->header;
    newType->elements_amount = oldType->elements_amount;
    newType->elements_used = oldType->elements_used;
    newType->row_length = oldType->row_length;
    newType->row = malloc(sizeof(char) * newType->row_length + 1);
    strcpy(newType->row, oldType->row);
    newType->str = malloc(sizeof(char*) * newType->elements_amount);
    for (int i = 0; i < newType->elements_amount; i++) {
        newType->str[i] = malloc(sizeof(char) * MAX_SIZE);
        strcpy(newType->str[i], oldType->str[i]);
    }
}

void selectByRow(Memory* memory, type* newType, char* line) {
//    SELECT_FROM_TYPES ROW(line)
//                copyType(newType, TYPE[i]);
//    end
    for (int i = 0; i < memory->used; i++) {
        if (strcmp(memory->Type[i]->row, line) == 0) {
            copyType(newType, TYPE[i]);
        }
    }
}

bool checkIfTypeStrEmpty(type* A) {
    for (int i = 0; i < A->elements_amount; i++) {
        if (strcmp(A->str[i], "") != 0) return false;
    }
    return true;
}

void minus(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                A->str[i][0] = '\0';
                break;
            }
        }
    }
}

bool empty(type* A) {
    if (A->elements_used == 0) return true;
    return true;
}

bool checkForRelationAndSetElementsInUniversum(Memory *memory) {
    char array[DEFAULT_SIZE] = {'R', 'S'};
    result* universum = createResult();
    selectByValue(memory, universum, 'U'); // in universum we have row for U
    type* Universum = malloc(sizeof(type));
    selectByRow(memory, Universum, universum->array[universum->used - 1]); //in Universum we have elements of U
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        result* Temp = createResult();
        selectByValue(memory, Temp, array[i]);     // in T we have row for R or S
        for (int j = 0; j < Temp->used; j++) {
            type* R = malloc(sizeof(type));
            selectByRow(memory, R, Temp->array[j]); // in R we have elements of R or S
            for (int d = 0; d < R->elements_used; d++) {
                bool found = false;
                for (int k = 0; k < Universum->elements_used; k++) { //check for elements of R or S in Universum
                    if (strcmp(R->str[d], Universum->str[k]) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    freeResult(universum);
                    freeResult(Temp);
                    freeType(R);
                    freeType(Universum);
                    return false;
                }
            }
            freeType(R);
        }
        freeResult(Temp);
    }
    freeResult(universum);
    freeType(Universum);
    return true;
}

void callFunctionByItName(char* name, Memory* executors) {
    printf("%s\n\n", name);
    for (int i = 0; i < executors->used; i++) {
        var_dump(executors->Type[i]);
    }
}

bool typeIsValid(type* A) {
    if (A->elements_used < 0) return false;
    return true;
}

void executeFunction(Memory* memory) {
    result* commands = createResult();

    selectByValue(memory, commands, 'C');
    foreachResult {
        bool commandIsValid = true;
        Memory* executive = malloc(sizeof(Memory));
        executive->size = DEFAULT_SIZE;
        executive->used = 0;
        executive->Type = malloc(sizeof(type) * DEFAULT_SIZE);


        type* command = malloc(sizeof(type));
        selectByRow(memory, command, commands->array[i]);
        printf("for command on %s row\n\n", command->row);
        for (int element = 1; element < command->elements_used; element++) {
            type* Type = malloc(sizeof(type));
            selectByRow(memory, Type, command->str[element]);
            if (!typeIsValid(Type)) {
                printf("[ERROR] while executing command on %s row\nSet / Relation / Universum are blank or do "
                       "not exist\n\n", command->row);
                free(Type);
                commandIsValid = false;
                break;
            }
            executive->Type[executive->used] = malloc(sizeof(type));
            copyType(executive->Type[executive->used], Type);
            if (++executive->used == executive->size) {
                executive->size++;
                executive->Type = realloc(executive->Type, sizeof(type*) * executive->size);
            }
            freeType(Type);
        }
        if (!commandIsValid) {
            freeType(command);
            freeMemory(executive);
            continue;
        }

        callFunctionByItName(command->str[0], executive);
        freeType(command);
        freeMemory(executive);
    }


    freeResult(commands);
}

int main(int argc, char **argv) {
    Memory *memory = createMemory();
    result* res = createResult();
    readFromFileV2(argv[1], memory);
//    for (int i = 0; i < memory->used; i++) var_dump(memory->Type[i]);

//    if (!checkForRelationAndSetElementsInUniversum(memory)) {
//        printf("ERROR: relation is not valid\n");
//        freeMemory(memory);
//        freeResult(res);
//        return 1;
//    }

    executeFunction(memory);

//    type* A= malloc(sizeof(type));
//    selectByRow(memory, A, "2");
//    var_dump(A);
//    freeType(A);

    freeMemory(memory);
    freeResult(res);
    return 0;

}