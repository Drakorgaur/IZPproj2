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

bool headerIsValid(char header) {
    if (header == 'U' || header == 'R' || header == 'C' || header == 'S') return true;
    return false;
}

bool valueIsValid(char value)
{
    if (((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9'))) return true;
    return false;
}

bool typeIsValid(type* A) {
    if (A->header == 'E') return false;
    if (A->elements_used < 0) return false;
    return true;
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

void dump(type *Type, int* cursor) {
    for (int i = *cursor; i < atoi(Type->row); i++) {
        printf("\n");
    }
    if (Type->header != 'F') {
        printf("%c ", Type->header);
        foreachElementInType {
            printf("%s ", Type->str[i]);
        }
    } else {
        printf("%s", Type->str[0]);
    }
    *cursor = atoi(Type->row);
}

//create new type
void createType(type* T) {
    T->header = 'N'; //NONE or NULL
    T->row = NULL;
    T->row_length = 0;
    T->str = NULL;
    T->elements_amount = 0;
    T->elements_used = 0;
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
            if (headerIsValid(symbol)) {
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
        if (valueIsValid(symbol)) strncat(wordBuffer, &symbol, 1);
        headerChecker = false;
    }
    memory->used++;
    fclose(file); free(wordBuffer);
}

void freeType(type* Type) {
    for (int i = 0; i < Type->elements_amount; i++) free(Type->str[i]);
    free(Type->str);
    free(Type->row);
    free(Type);
}

void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->used; i++) {
        for (int j = 0; j < memory->Type[i]->elements_amount; j++) freeType(memory->Type[i]);
        free(memory->Type[i]->str);
        free(memory->Type[i]->row);
    }
    free(memory->Type);
    free(memory);
}

void freeResult(result* res) {
    for (int i = 0; i < res->size; i++) free(res->array[i]);
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

bool empty(type* A) {
    if (A->elements_used == 0) return true;
    return false;
}

char* card(type* A) {
    char* str = malloc(sizeof(char) * MAX_SIZE);
    sprintf(str, "%d", A->elements_used);
    return str;
}

char* complement(type* A, type* U) {
    return "";
}

/*
 * TODO: test
 */
char* union_(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        printf("%s ", A->str[i]);
    }
    for (int j = 0; j < B->elements_used; j++) {
        for (int i = 0; i < A->elements_used; i++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                break;
            }
        }
        printf("%s ", B->str[j]);
    }
    return "";
}

/*
 * TODO: test
 */
char* intersect(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                printf("%s ", A->str[i]);
                break;
            }
        }
    }
    return "";
}

/*
 * TODO: test
 */
char* minus(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                strcpy(A->str[i], "");
                break;
            }
        }
        printf("%s ", A->str[i]);
    }
    return "";
}

/*
 * TODO: test
 */
char* subseteq(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                break;
            }
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* subset(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                break;
            }
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* equals(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                break;
            }
        }
    }
    return "true";
}

/*
 * -------------------RELATIONS-------------------
 */

/*
 * TODO: test
 */
char* reflexive(type* A) {
    for (int i = 0; i < A->elements_used; i++) {
        if (strcmp(A->str[i], A->str[i]) != 0) return false;
    }
    return "true";
}

/*
 * TODO: test
 */
char* symmetric(type* A) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < A->elements_used; j++) {
            if (strcmp(A->str[i], A->str[j]) != 0) return false;
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* antisymmetric(type* A) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < A->elements_used; j++) {
            if (strcmp(A->str[i], A->str[j]) == 0 && i != j) return false;
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* transitive(type* A) {
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < A->elements_used; j++) {
            for (int k = 0; k < A->elements_used; k++) {
                if (strcmp(A->str[i], A->str[j]) == 0 && strcmp(A->str[j], A->str[k]) == 0 && i != j && j != k) return false;
            }
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* function(type* A) {
    return "blabla";
}

/*
 * TODO: test
 */
char* domain(type* A) {
    return "false";
}

/*
 * TODO: test
 */
char* injective(type* C, type* A, type* B) {
    return "false";
}

/*
 * TODO: test
 */
char* surjective(type* C, type* A, type* B) {
    return "false";
}

/*
 * TODO: test
 */
char* bijective(type* C, type* A, type* B) {
    return "false";
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

char* callFunctionByItName(char* name, Memory* executors, type* U) {
    int A = 0;
    int B = 1;
    int C = 2;
    if (strcmp(name, "empty") == 0) {
        if (empty(executors->Type[0])) { return "true"; } else { return "false"; }
    }
    if (strcmp(name, "card") == 0) return card(executors->Type[A]);
    if (strcmp(name, "complement") == 0) return complement(executors->Type[A], U);
    if (strcmp(name, "union") == 0) return union_(executors->Type[A], executors->Type[B]);
    if (strcmp(name, "intersect") == 0) return intersect(executors->Type[A], executors->Type[B]);
    if (strcmp(name, "minus") == 0) return minus(executors->Type[A], executors->Type[B]);
    if (strcmp(name, "subseteq") == 0) return subseteq(executors->Type[A], executors->Type[B]);
    if (strcmp(name, "subset") == 0) return subset(executors->Type[A], executors->Type[B]);
    if (strcmp(name, "equals") == 0) return equals(executors->Type[A], executors->Type[B]);

    if (strcmp(name, "reflexive") == 0) return reflexive(executors->Type[A]);
    if (strcmp(name, "symmetric") == 0) return symmetric(executors->Type[A]);
    if (strcmp(name, "antisymmetric") == 0) return antisymmetric(executors->Type[A]);
    if (strcmp(name, "transitive") == 0) return transitive(executors->Type[A]);
    if (strcmp(name, "function") == 0) return function(executors->Type[A]);
    if (strcmp(name, "domain") == 0) return domain(executors->Type[A]);
    if (strcmp(name, "injective") == 0) return injective(executors->Type[A], executors->Type[B], executors->Type[C]);
    if (strcmp(name, "surjective") == 0) return surjective(executors->Type[A], executors->Type[B], executors->Type[C]);
    if (strcmp(name, "bijective") == 0) return bijective(executors->Type[A], executors->Type[B], executors->Type[C]);
    return "UNEXPECTED ERROR OCCURRED";
}

int getMaxLength(Memory* memory) {
    int max = 0;
    for (int i = 0; i < memory->used; i++) {
        if (memory->Type[i]->elements_used > max) {
            max = memory->Type[i]->elements_used;
        }
    }
    return max;
}

void executeFunction(Memory* memory) {
    int size = getMaxLength(memory);

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
            Type->header = 'E';
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

        result* Universum = createResult();
        selectByValue(memory, Universum, 'U');
        type* U = malloc(sizeof(type));
        selectByRow(memory, U, Universum->array[0]);

        char* str = malloc(sizeof(char) * (size * MAX_SIZE));
        strcpy(str, callFunctionByItName(command->str[0], executive, U));
        for (int j = 0; j < memory->used; j++) {
            if (strcmp(memory->Type[j]->row, commands->array[i]) == 0) {
                memory->Type[j]->header = 'F';
                for (int k = 0; k < memory->Type[j]->elements_amount; k++) {
                    memory->Type[j]->str[k] = realloc(memory->Type[j]->str[k], sizeof(char*) * size * MAX_SIZE);
                }
                strcpy(memory->Type[j]->str[0], str);
            }
        }

        free(str);
        freeType(U);
        freeResult(Universum);
        freeType(command);
        freeMemory(executive);
    }
    freeResult(commands);
}

int main(int argc, char **argv) {
//    Memory *memory = createMemory();
//    result* res = createResult();
//    readFromFileV2(argv[1], memory);

//    if (!checkForRelationAndSetElementsInUniversum(memory)) {
//        printf("ERROR: relation is not valid\n");
//        freeMemory(memory);
//        freeResult(res);
//        return 1;
//    }

//    executeFunction(memory);

    type* A= malloc(sizeof(type));
    createType(A);
//    selectByRow(memory, A, "2");
//    var_dump(A);
    freeType(A);

//    int cursor = 1;
//    for (int i = 0; i < memory->used; i++) dump(memory->Type[i], &cursor);

//    freeResult(res);
//    freeMemory(memory);
    return 0;

}