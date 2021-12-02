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
    if ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9')
    || (value == '(' || value == ')')) return true;
    return false;
}

bool typeIsValid(type* A) {
    if (A->header == 'E') return false;
    if (A->elements_used < 0) return false;
    return true;
}

bool wordIsNotrestricted(char* word) {
    char restricted[][31] = {"true", "false", "empty", "card", "complement", "union", "intersect", "minus",
                               "subseteq", "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive",
                               "function", "domain", "codomain", "injective", "surjective", "bijective"
                               };
    int size = sizeof(restricted) / sizeof(restricted[0]);
    for (int i = 0; i <= size; i++) {
        if (strcmp(word, restricted[i]) == 0) return false;
    }
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

void createRow(type* T) {
    T->row = malloc(sizeof(char) * 3);
}
void createStr(type* T) {
    T->str = malloc(sizeof(char*) * DEFAULT_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        T->str[i] = malloc(sizeof(char) * DEFAULT_SIZE);
    }
}
void createStrArray(type* T) {
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        T->str[i] = malloc(sizeof(char) * MAX_SIZE);
        strcpy(T->str[i], "");
    }
}

void copyType(type* newType, type* oldType) {
    newType->header = oldType->header;
    newType->elements_amount = oldType->elements_used;
    newType->elements_used = oldType->elements_used;
    newType->row = malloc(sizeof(char) * 3);
    strcpy(newType->row, oldType->row);
    newType->str = malloc(sizeof(char*) * newType->elements_used);
    for (int i = 0; i < newType->elements_used; i++) {
        newType->str[i] = malloc(sizeof(char) * MAX_SIZE);
        strcpy(newType->str[i], oldType->str[i]);
    }
}

//create new type
void createType(type* T) {
    T->header = 'N'; //NONE or NULL
    createRow(T);
    createStr(T);
    createStrArray(T);
    T->elements_amount = DEFAULT_SIZE;
    T->elements_used = 0;
}

type** initTypeArray() {
    type** Type = malloc(sizeof(type*) * DEFAULT_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i] = malloc(sizeof(type));
        createType(Type[i]);
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
    createType(memory->Type[memory->used]);
}

void resizeStr(type* T) {
    T->str = realloc(T->str, sizeof(char*) * (T->elements_amount + 1));
    T->str[T->elements_amount] = malloc(sizeof(char) * MAX_SIZE);
    T->elements_amount++;
}

bool readFromFileV2(char* filename, Memory* memory) {
    FILE* file = fopen(filename, "r");

    int row  = 1;
    bool headerChecker = true, universumCheck = false;
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
                if (universumCheck) {
                    if (!wordIsNotrestricted(wordBuffer)) {
                        fprintf(stderr, "Universum has restricted word");
                        fclose(file);
                        free(wordBuffer);
                        return false;
                    }
                }
                strcpy(memory->Type[memory->used]->str[memory->Type[memory->used]->elements_used], wordBuffer);
                if (strcmp(wordBuffer, "") != 0) {
                    memory->Type[memory->used]->elements_used++;
                }
                strcpy(wordBuffer, "");
                if (memory->Type[memory->used]->elements_used == memory->Type[memory->used]->elements_amount)
                    resizeStr(memory->Type[memory->used]);
                wordSize = 0;
            }
            if (symbol == '\n') {
                row++;
                wordSize = 0;
                symbol = fgetc(file);
                if (!headerChecker) {
                    if (universumCheck) {
                        if (!wordIsNotrestricted(wordBuffer)) {
                            fprintf(stderr, "Universum has restricted word");
                            fclose(file);
                            free(wordBuffer);
                            return false;
                        }
                    }
                    strcpy(memory->Type[memory->used]->str[memory->Type[memory->used]->elements_used], wordBuffer);
                    if (strcmp(wordBuffer, "") != 0)
                        memory->Type[memory->used]->elements_used++;
                    strcpy(wordBuffer, "");
                    if (memory->Type[memory->used]->elements_used == memory->Type[memory->used]->elements_amount)
                        resizeStr(memory->Type[memory->used]);
                }
                headerChecker = true;
                universumCheck = false;
                continue;
            }
            symbol = fgetc(file);
        }
        if (wordSize == MAX_SIZE) {
            fclose(file);
            free(wordBuffer);
            fprintf(stderr, "Error: element couldn't have size more than 30\n");
            return false;
        }
        if (headerChecker) {
            if (headerIsValid(symbol)) {
                if (++memory->used == memory->size) resizeMemory(memory);
                memory->Type[memory->used]->header = symbol;
                if (memory->Type[memory->used]->header == 'U') {
                    universumCheck = true;
                }
                sprintf(memory->Type[memory->used]->row, "%d", row);
                continue;
            } else {
                if(feof(file)) {
                    continue;
                }
                fprintf(stderr, "Error: header is invalid\n");
                free(wordBuffer);
                fclose(file);
                return false;
            }
        }
        if (valueIsValid(symbol)) strncat(wordBuffer, &symbol, 1);
        headerChecker = false;
    }
    memory->used++;
    fclose(file); free(wordBuffer);
    return true;
}

void freeType(type* Type) {
    for (int i = 0; i < Type->elements_amount; i++) {
        free(Type->str[i]);
    }
    free(Type->row);
    free(Type->str);
    free(Type);
}

void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->size; i++) {
        freeType(memory->Type[i]);
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

void selectByRow(Memory* memory, type* newType, char* line) {
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

char* empty(type* A, char* str) {
    if (A->elements_used == 0) return "true";
    return "false";
}

int card(type* A) {
    return A->elements_used;
}

char* complement(type* A, type* U, char* str) {
    return "";
}

/*
 * TODO: test
 */
char* union_(type* A, type* B, char* str) {
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
char* intersect(type* A, type* B, char* str) {
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
char* minus(type* A, type* B, char* str) {
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

void getUnique(type* A, char elements[][31], int* size, int start, int interval) {
    for (int i = start; i < A->elements_used; i+=interval) {
        bool found = false;
        for (int j = 0; j < *size; j++) {
            if (strcmp(A->str[i], elements[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            if (strcmp(A->str[i], "") == 0) {
                continue;
            }
            strcpy(elements[*size], A->str[i]);
            (*size)++;
        }
    }
}
/*
 * TODO: DONE/test - test j < size to j < size + 1;
 */
char* reflexive(type* R) {
    if (!(R->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    int size = 0;
    char elements[R->elements_used][31];
    strcpy(elements[size], R->str[0]);
    size++;
    getUnique(R, elements, &size, 1, 1);
    for (int j = 0; j < size; j++) {
        bool found = false;
        for (int i = 0; i < R->elements_used; i += 2) {
            if (strcmp(elements[j], R->str[i]) == 0) {
                if (strcmp(elements[j], R->str[i + 1]) == 0) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            return "false";
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* symmetric(type* A) {
    if (!(A->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    for (int i = 0; i < A->elements_used; i+=2) {
        bool found = false;
        if (strcmp(A->str[i], A->str[i+1]) == 0) {
            continue;
        }
        for (int j = 1; j < A->elements_used; j+=2) {
            if (strcmp(A->str[i+1], A->str[j-1]) == 0) {
                if (strcmp(A->str[j], A->str[i]) == 0) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            return "false";
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* antisymmetric(type* A) {
    if (!(A->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    for (int i = 0; i < A->elements_used; i+=2) {
        bool found = false;
        if (strcmp(A->str[i], A->str[i+1]) == 0) {
            continue;
        }
        for (int j = 1; j < A->elements_used; j+=2) {
            if (strcmp(A->str[i+1], A->str[j-1]) == 0) {
                if (strcmp(A->str[j], A->str[i]) == 0) {
                    return "false";
                }
            }
        }
    }
    return "true";
}

/*
 * TODO: NOT DONE - NEED TO CHECK AND TEST
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
    if (!(A->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    for (int i = 0; i < A->elements_used; i+=2) {
        for (int j = 0; j < A->elements_used; j+=2) {
            if (j == i) continue;
            if (strcmp(A->str[i], A->str[j]) == 0) {
                return "false";
            }
        }
    }
    return "true";
}

/*
 * TODO: DONE/test
 *
 * TODO: create error message
 *
 * TODO: test isFunction
 */
void domain(type* R, char* str) {
    char* isFunction = function(R);
    if (strcmp(isFunction, "false") == 0) {
        fprintf(stderr, "Error: relation is not a function\n");
        return;
    }
    strcpy(str, "");
    int size = 0;
    char elements[R->elements_used][31];
    getUnique(R, elements, &size, 0, 2);
    for (int i = 0; i <= size; i++) {
        strcat(str, elements[i]);
        if (strcmp(elements[i], "") == 0) {
            continue;
        }
        strcat(str, " ");
    }
}

/*
 * TODO: test isFunction
 */
void codomain(type* R, char* str) {
    char* isFunction = function(R);
    if (strcmp(isFunction, "false") == 0) {
        fprintf(stderr, "Error: relation is not a function\n");
        return;
    }
    strcpy(str, "");
    int size = 0;
    char elements[R->elements_used][31];
    getUnique(R, elements, &size, 0, 2);
    for (int i = 0; i <= size; i++) {
        strcat(str, elements[i]);
        if (strcmp(elements[i], "") == 0) {
            continue;
        }
        strcat(str, " ");
    }
}

/*
 * TODO: test
 */
char* injective(type* R, type* A, type* B) {
    if (!(R->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    if (!(A->elements_used) || !(B->elements_used)) {
        fprintf(stderr, "Error: empty set(s)\n");
        return "";
    }
    /*
     * TODO: create a helper
     */
    int sizeA = 0, sizeB = 0;
    char elementsA[A->elements_used][31], elementsB[B->elements_used][31];
    strcpy(elementsA[sizeA], A->str[0]);
    strcpy(elementsB[sizeB], B->str[0]);
    sizeA++;
    sizeB++;
    getUnique(A, elementsA, &sizeA, 1, 1);
    getUnique(B, elementsB, &sizeB, 1, 1);
    for (int i = 0; i < R->elements_used; i+=2) {
        bool found = false;
        for (int j = 0; j <= sizeA; j++) {
            if (strcmp(R->str[i], elementsA[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
//            fprintf(stderr, "Error: relation must have elements on 1st place from set A\n");
            return "Error: relation must have elements from set A";
        }
        for (int j = 0; j < R->elements_used; j+=2) {
            if (j == i) continue;
            if (strcmp(R->str[i], R->str[j]) == 0) {
                return "false";
            }
        }
    }
    for (int i = 1; i < R->elements_used; i+=2) {
        bool found = false;
        for (int j = 0; j <= sizeB; j++) {
            if (strcmp(R->str[i], elementsB[j]) == 0) {
                if (found) {
                    return "false";
                }
                found = true;
                break;
            }
        }
        if (!found) {
//            fprintf(stderr, "Error: relation must have elements on 2nd place  from set B\n");
            return "Error: relation must have elements from set B";
        }
        for (int j = 1; j < R->elements_used; j+=2) {
            if (j == i) continue;
            if (strcmp(R->str[i], R->str[j]) == 0) {
                return "false";
            }
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* surjective(type* R, type* A, type* B) {
    if (!(R->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    if (!(A->elements_used) || !(B->elements_used)) {
        fprintf(stderr, "Error: empty set(s)\n");
        return "";
    }
    /*
     * TODO: create a helper
     */
    int sizeA = 0;
    char elementsA[A->elements_used][31];
    strcpy(elementsA[sizeA], A->str[0]);
    sizeA++;
    getUnique(A, elementsA, &sizeA, 1, 1);
    for (int i = 0; i < sizeA; i++) {
        bool found = false;
        for (int j = 0; j < R->elements_used; j+=2) {
            if (strcmp(R->str[j], elementsA[i]) == 0) {
                if (found) {
                    return "false";
                }
                found = true;
                break;
            }
        }
        if (!found) {
            return "false";
        }
    }
    return "true";
}

/*
 * TODO: test
 */
char* bijective(type* R, type* A, type* B) {
    if (!(R->elements_used)) {
        fprintf(stderr, "Error: empty relation\n");
        return "";
    }
    if (!(A->elements_used) || !(B->elements_used)) {
        fprintf(stderr, "Error: empty set(s)\n");
        return "";
    }
    /*
     * TODO: create a helper
     */
    int sizeA = 0;
    int sizeB = 0;
    char elementsA[A->elements_used][31];
    char elementsB[B->elements_used][31];
    strcpy(elementsA[sizeA], A->str[0]);
    strcpy(elementsB[sizeB], B->str[0]);
    sizeA++;
    sizeB++;
    getUnique(A, elementsA, &sizeA, 1, 1);
    getUnique(B, elementsB, &sizeB, 1, 1);
    for (int i = 0; i < sizeA; i++) {
        bool found = false;
        for (int j = 0; j < R->elements_used; j+=2) {
            if (strcmp(R->str[j], elementsA[i]) == 0) {
                if (found) {
                    return "false";
                }
                found = true;
            }
        }
        if (!found) {
            return "false";
        }
    }
    for (int i = 0; i < sizeB; i++) {
        bool found = false;
        for (int j = 1; j < R->elements_used; j+=2) {
            if (strcmp(R->str[j], elementsB[i]) == 0) {
                if (found) {
                    return "false";
                }
                found = true;
            }
        }
        if (!found) {
            return "false";
        }
    }
    return "true";
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

bool checkHeader(type* A, char header) {
    if (A->header == header) return false;
    return true;
}

void callFunctionByItName(char* name, Memory* executors, type* U, char* str) {
    int A = 0;
    int B = 1;
    int C = 2;
    if (executors->used == 1) {
        if (strcmp(name, "empty") == 0) {
            if (checkHeader(executors->Type[0], 'S')) {
                fprintf(stderr, "Error: empty() can be used only with sets\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, empty(executors->Type[0], str)); return;
        }
        if (strcmp(name, "card") == 0) {
            if (checkHeader(executors->Type[0], 'S')) {
                fprintf(stderr, "Error: card() can be used only with sets\n");
                strcpy(str, "FAIL\n");
                return;
            }
            sprintf(str, "%d", card(executors->Type[0])); return;
        }
        if (strcmp(name, "complement") == 0) {
            if (checkHeader(executors->Type[0], 'S')) {
                fprintf(stderr, "Error: complement() can be used only with sets\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, complement(executors->Type[0], U, str)); return;
        }
        if (strcmp(name, "reflexive") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: reflexive() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, reflexive(executors->Type[0])); return;
        }
        if (strcmp(name, "symmetric") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: symmetric() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, symmetric(executors->Type[0]));
            return;
        }
        if (strcmp(name, "antisymmetric") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: antisymmetric() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, antisymmetric(executors->Type[0])); return;
        }
        if (strcmp(name, "transitive") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: transitive() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, transitive(executors->Type[0])); return;
        }
        if (strcmp(name, "function") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: function() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, function(executors->Type[0])); return;
        }
        if (strcmp(name, "domain") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: domain() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            domain(executors->Type[0], str); return;
        }
        if (strcmp(name, "codomain") == 0) {
            if (checkHeader(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: codomain() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return;
            }
            codomain(executors->Type[0], str); return;
        }
    }

    if (executors->used == 2) {
        if (strcmp(name, "union") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: union() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, union_(executors->Type[0], executors->Type[1], str)); return;
        }
        if (strcmp(name, "intersect") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: intersect() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, intersect(executors->Type[0], executors->Type[1], str)); return;
        }
        if (strcmp(name, "minus") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: minus() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, minus(executors->Type[0], executors->Type[1], str)); return;
        }
        if (strcmp(name, "subseteq") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: subseteq() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, subseteq(executors->Type[0], executors->Type[1])); return;
        }
        if (strcmp(name, "subset") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: subset() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, subset(executors->Type[0], executors->Type[1])); return;
        }
        if (strcmp(name, "equals") == 0) {
            if (checkHeader(executors->Type[0], 'S') && checkHeader(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with sets");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, equals(executors->Type[0], executors->Type[1])); return;
        }
    }

    if (executors->used == 3) {
        if (strcmp(name, "injective") == 0)
        {
            if (checkHeader(executors->Type[0], 'R') && checkHeader(executors->Type[1], 'S')
                                                                    && checkHeader(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, injective(executors->Type[0], executors->Type[1], executors->Type[2])); return;
        }
        if (strcmp(name, "surjective") == 0) {
            if (checkHeader(executors->Type[0], 'R') && checkHeader(executors->Type[1], 'S')
                                                                    && checkHeader(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, surjective(executors->Type[0], executors->Type[1], executors->Type[2])); return;
        }
        if (strcmp(name, "bijective") == 0) {
            if (checkHeader(executors->Type[0], 'R') && checkHeader(executors->Type[1], 'S')
                                                                    && checkHeader(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return;
            }
            strcpy(str, bijective(executors->Type[0], executors->Type[1], executors->Type[2])); return;
        }
    }
    strcpy(str, "Error: not right function atributes");
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
    char* str = malloc(sizeof(char) * (size * MAX_SIZE));
    foreachResult {
        bool commandIsValid = true;
        Memory* executive = createMemory();
        for (int j = 0; j < executive->size; j++) {
            executive->Type[j] = malloc(sizeof(type));
            createType(executive->Type[j]);
        }

        type* command = malloc(sizeof(type));
        selectByRow(memory, command, commands->array[i]);
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
            copyType(executive->Type[executive->used], Type);
            if (++executive->used == executive->size) {
                resizeMemory(executive);
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
        callFunctionByItName(command->str[0], executive, U, str);
        for (int j = 0; j < memory->used; j++) {
            if (strcmp(memory->Type[j]->row, commands->array[i]) == 0) {
                memory->Type[j]->header = 'F';
                for (int k = 0; k < memory->Type[j]->elements_amount; k++) {
                    memory->Type[j]->str[k] = realloc(memory->Type[j]->str[k], sizeof(char*) * size * MAX_SIZE);
                }
                strcpy(memory->Type[j]->str[0], str);
            }
        }
        freeType(U);
        freeType(command);
        freeResult(Universum);
        freeMemory(executive);
    }
    free(str);
    freeResult(commands);
}

bool checkAndRefactorRelations(Memory* memory) {
    printf("\n");
    for (int i = 0; i < memory->used; i++) {
        if (memory->Type[i]->header == 'R') {
            if (memory->Type[i]->elements_used % 2) {
                fprintf(stderr, "Relation have odd amount of elements\n");
                return false;
            }
            for (int j = 0; j < memory->Type[i]->elements_used; j++) {
                char* str = "(";
                char* str2 = ")";
                char* p = strstr(memory->Type[i]->str[j], str);
                char* p2 = strstr(memory->Type[i]->str[j], str2);
                if (!(j % 2) && p != NULL) {
                    /*
                     * TODO: fix overlapping
                     */
                    p++;
                    strcpy(memory->Type[i]->str[j], p);
                    continue;
                }
                if (j % 2 && p2 != NULL) {
                    char* temp = memory->Type[i]->str[j];
                    int size = strlen(temp);
                    temp[size - 1] = '\0';
                    strcpy(memory->Type[i]->str[j], temp);
                    continue;
                }
                fprintf(stderr, "Relation have wrong input format\n");
                return false;
            }
        }
    }
    return true;
}

void testFunction() {
    Memory* memory = malloc(sizeof(Memory));
    memory->size = 1;
    memory->used = 0;
    memory->Type = malloc(sizeof(type) * DEFAULT_SIZE);
    memory->Type[memory->used] = malloc(sizeof(type));
    freeMemory(memory);
}

int main(int argc, char **argv) {
    (void)argc;
    Memory *memory = createMemory();
//    if (!readFromFileV2(argv[1], memory)) {
//        freeMemory(memory);
//        return 1;
//    }
////    int cursor = 1;
////    for (int i = 0; i < memory->used; i++) dump(memory->Type[i], &cursor);
//    if (!checkAndRefactorRelations(memory)) {
//        freeMemory(memory);
//        return 1;
//    }
//    result* res = createResult();
//    if (!checkForRelationAndSetElementsInUniversum(memory)) {
//        fprintf(stderr, "ERROR: set/relation have element that is not in univesum\n");
//        freeMemory(memory);
//        freeResult(res);
//        return 1;
//    }
//    executeFunction(memory);
//    int cursor = 1;
//    for (int i = 0; i < memory->used; i++) dump(memory->Type[i], &cursor);
//    freeResult(res);
    freeMemory(memory);
//    testFunction();
    return 0;
}