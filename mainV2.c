#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * we create boolean type
 */
typedef unsigned short bool;
#define true 1
#define false 0

/**
 * we define DEFAULT_SIZE and MAX_SIZE
 *      DEFAULT_SIZE is used to define the size of the array
 *      MAX_SIZE is used to define (char*) element
 */
#define DEFAULT_SIZE 2
#define MAX_SIZE 31

/**
 * Structure type is created to save lines.
 * lines contains:
 *      header(U/R/S/C)
 *          --------------------------------------------------
 *          U - universum
 *          R - relation
 *          S - set
 *          C - command
 *          --------------------------------------------------
 *          E - empty/error
 *          F - function
 *          --------------------------------------------------
 *      str -> elements(string).
 *          str is 2D array to save strings as array
 *      elements_amount -> amount of elements in str
 *      elements_used   -> amount of used elements in str
 *
 *  elements_used is check if we need resize our str in struct.
 *  we resize str if elements_used == elements_amount
 */
typedef struct {
    char header;
    char* row;
    char** str;
    int elements_amount;
    int elements_used;
} type;

/**
 * (**Type) - array of pointers to type objects
 *
 * Structure Memory is created to save all Types as array
 * and check if we need resize our array of Types.
 * to check it we have:
 *      int size -> current size of array
 *      int used -> current used elements in array
 *
 *  we resize **Type if used == size
 */
typedef struct {
    type **Type;
    int size;
    int used;
} Memory;

/**
 * Structure result is structure to save type-objects' rows
 *
 * This structure is support-structure for functions:
 *      selectByValue
 *      selectByRow
 *
 * as in previous structures
 *      int size
 *      int used
 * help us to check if we need resize our array of rows.
 */
typedef struct {
    char** array;
    int size;
    int used;
} result;

/**
 * "headerIsValid" method is checking if provided header is valid
 * to write it in our type-object.
 *
 * @param header (char)
 * @return
 *      True if header is valid
 *      False if header is not valid
 */
bool headerIsValid(char header) {
    if (header == 'U' || header == 'R' || header == 'C' || header == 'S') return true;
    return false;
}

/**
 * "valueIsValid" method is checking if provided symbol is valid and
 * if we can write it to our type-object.
 *
 * This function checks symbols for
 *      Universum,
 *      Sets,
 *      Relation;
 * ---NOT-FOR-----
 *      Commands
 *
 * @param value (char)
 * @return
 *      True if symbol is valid
 *      False if symbol is not valid
 */
bool valueIsValid(char value)
{
    if ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z')
    || (value == '(' || value == ')')) return true;
    return false;
}

/**
 * "commandValueIsValid" method is checking if provided symbol is valid and
 * we can write it to our type-object that contains command line.
 *
 * it used only for commands line
 *
 * @param value (char)
 * @return
 *      True if symbol is valid
 *      False if symbol is not valid
 */
bool commandValueIsValid(char value)
{
    if ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9')
        || (value == '(' || value == ')')) return true;
    return false;
}

/**
 * "typeIsValid" method is checking if provided type is right-created.
 *
 * Type that will be created in some of our methods would have header 'E'
 *      heaeder 'E' is ERROR or EMPTY
 *  Also we check if elements _used is less than zero to check if we have set elements_used
 *
 * @param A (type-object)
 * @return
 *      True if type is valid
 *      False if type is not valid
 */
bool typeIsValid(type* A) {
    if (A->header == 'E' && A->elements_used <  0 ) return false;
    return true;
}

/**
 * in "wordIsNotrestricted" method we check if provided word is not in array of restricted words
 *
 * @param word (char*) - to get string
 * @return
 *      True if word is not restricted
 *      False if word is restricted
 */
bool wordIsNotrestricted(char* word) {
    char restricted[][31] = {"true", "false", "empty", "card", "complement", "union", "intersect", "minus",
                               "subseteq", "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive",
                               "function", "domain", "codomain", "injective", "surjective", "bijective"
                               };
    int size = sizeof(restricted) / sizeof(restricted[0]);
    for (int i = 0; i < size; i++) {
        if (strcmp(word, restricted[i]) == 0) return false;
    }
    return true;
}

/**
 * "dump" method is print in provided by project-condition form.
 *
 * @param Type (type-object)
 * @param cursor (int*)
 *      cursor is int that have number of row of previous object
 *      so if we have some empty lines in out .txt file
 *      we could reproduce them in our output - STDOUT
 *      (our first cycle)
 *
 * next condition checks of header is not F
 *      F - function
 * if it is NOT FUNCTION:
 *      1) we print heaeder of our type-object Type;
 *      2) we check if this line will be line of relation(R)
 *          IF RELATION:
 *          2.1.1) before each even elemnt we print '('
 *          2.1.2) after each odd elemnt we print ') ' with space after to separate elements
 *                   '(' and ')' were deleted by method checkAndRefactorRelations(read below)
 *
 *          IF NOT RELATION:
 *          2.2.1) we print all elements of our type-object Type and separate them with space
 * if it is FUNCTION:
 *      1) we print our elements in str[0] separeted with space
 *          this str[0] of our type-object Type resized to (MAX_LENGHT * (max(of elements_used))) to be able to obtain
 *          all elements of our type-object(s)
 *
 *  pointer to cursor gets updated by value of this type-object Type's row
 */
void dump(type *Type, int* cursor) {
    for (int i = *cursor; i < atoi(Type->row); i++) {
        printf(" \n");
    }
    if (Type->header != 'F') {
        printf("%c ", Type->header);
        if (Type->header == 'R') {
            for (int i = 0; i < Type->elements_used; i++) {
                if (!(i % 2)) {
                    printf("(");
                    printf("%s ", Type->str[i]);
                } else {
                    printf("%s", Type->str[i]);
                    printf(") ");
                }
            }
        } else {
            //foreach Element in Type
            for (int i = 0; i < Type->elements_used; i++) {
                printf("%s ", Type->str[i]);
            }
        }
    } else {
        printf("%s ", Type->str[0]);
    }
    *cursor = atoi(Type->row);
}

/**
 * "createRow" method allocates our row of type-object T
 *      this method was created separately to check memory leaking by valgrind
 *
 * @param T (type-object)
 */
void createRow(type* T) {
    T->row = malloc(sizeof(char) * 3);
}

/**
 * "createStr" method allocates our str-array of type-object T
 *      we have to allocate memory for our str-array
 *          and then
 *      we have to allocate memory for each element of our str-array
 *
 *      this method was created separately to check memory leaking by valgrind
 *
 * @param T (type-object)
 */
void createStr(type* T) {
    T->str = malloc(sizeof(char*) * T->elements_amount);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        T->str[i] = malloc(sizeof(char) * MAX_SIZE);
    }
}

/**
 *  "copyType" method provides ability to copy data from our type-object oldType to our type-object newType
 *
 *      To use this method we have only allocate memory for our type-object newType and provide it to this method
 * @param newType (type-object)
 * @param oldType (type-object)
 */
void copyType(type* newType, type* oldType) {
    newType->header = oldType->header;
    newType->elements_amount = oldType->elements_used;
    newType->elements_used = oldType->elements_used;
    createRow(newType);
    strcpy(newType->row, oldType->row);
    newType->str = malloc(sizeof(char*) * newType->elements_used);
    for (int i = 0; i < newType->elements_used; i++) {
        newType->str[i] = malloc(sizeof(char) * MAX_SIZE);
        strcpy(newType->str[i], oldType->str[i]);
    }
}

/**
 * "createType" is method that creates our type-object T
 *
 * @param T (type-object)
 *
 *      we set header of object as 'N' what means NONE/NULL
 *          so we know that object is empty
 *      as standart was choosen to create array [2][31] so we set T->elements_amount to DEFAULT_SIZE
 *      and call createStr method to create it with chosen T->elements_amount param
 */
void createType(type* T) {
    T->header = 'N';
    T->elements_amount = DEFAULT_SIZE;
    createRow(T);
    createStr(T);
    T->elements_used = 0;
}

/**
 * "initTypeArray" method creates our type-object array and return it
 *      as standart was choosen to create 2D-arrays as [2][x] so we allocate
 *      **Type with DEFAULT_SIZE
 *
 * this method is used only for createMemory method, because only memory-object must have
 * 2D-array of type-objects
 *
 * @return 2D-Type array
 */
type** initTypeArray() {
    type** Type = malloc(sizeof(type*) * DEFAULT_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        Type[i] = malloc(sizeof(type));
        createType(Type[i]);
    }
    return Type;
}

/**
 * "createMemory" method creates our memory-object
 *      as standart was choosen to create 2D-arrays as [2][x] so we set Memory-object size to DEFAULT_SIZE
 *
 * @return memory-object
 */
Memory* createMemory() {
    Memory* memory = malloc(sizeof(Memory));
    memory->Type = initTypeArray();
    memory->size = DEFAULT_SIZE;
    memory->used = 0;
    return memory;
}

/**
 * "resizeMemory" method re-allocate memory for our memory-object
 *      we increase our memory-object size by 1
 *      and increase our memory-object Type-array size by this increment
 *          then we have to allocate memory for our new Type-object
 *          and then we create empty type-object
 *
 * @param memory (memory-object)
 */
void resizeMemory(Memory* memory) {
    memory->size++;
    memory->Type = realloc(memory->Type, sizeof(type*) * memory->size);
    memory->Type[memory->used] = malloc(sizeof(type));
    createType(memory->Type[memory->used]);
}

/**
 * "resizeStr" method re-allocate memory for our str-array of type-object T
 *      we realloc our str-array by 1
 *      initialize new element of str-array with malloc
 *          and increase our elements_size by 1
 * @param T (type-object)
 */
void resizeStr(type* T) {
    T->str = realloc(T->str, sizeof(char*) * (T->elements_amount + 1));
    T->str[T->elements_amount] = malloc(sizeof(char) * MAX_SIZE);
    T->elements_amount++;
}

/**
 * "readFromFileV2" method reads file, checks input and write data to our type-objects in memory-object
 *      we provide file name that we had to get as program argument
 *      we checks if file is empty
 *
 * variables:
 *      int row                number of row in file
 *      char symbol            symbol that we read from file
 *      char* wordBuffer       buffer that contains word from file, NOT a symbol
 *      bool headerChecker     check if we wait for header from input, that gains true if we have new line
 *      bool universumCheck    check if we now write in unviersum to prevent writing restricted words
 *
 *      we use
 *          memory->used = -1; because we had to check if line would be empty at start or we would have empty spaces
 *          after word was copied from buffer to type-object we clear byffer by method strcpy with ""
 * @param filename
 * @param memory
 * @return
 */
bool readFromFileV2(char* filename, Memory* memory) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed: to open file %s\n", filename);
        return false;
    }
    int row = 1;
    bool headerChecker = true, universumCheck = false;
    char symbol, *wordBuffer;
    wordBuffer = malloc(sizeof(char) * MAX_SIZE);
    strcpy(wordBuffer, "");
    memory->used = -1;  //
    for (int wordSize = 0; !feof(file); wordSize++) {
        if (row == 1000) {
            fprintf(stderr, "Failed: rows is more than 1000 %s\n", filename);
            fclose(file);
            free(wordBuffer);
            return false;
        }
        symbol = fgetc(file);
        while (symbol == ' ' || symbol == '\n') {
            /**
             * this condition skips spaces
             */
            if (symbol == ' ') {
                if (headerChecker) {
                    /**
                     * we skip spaces if we waiting for header
                     */
                    if (memory->used == -1) {
                        symbol = fgetc(file);
                        continue;
                    }
                    symbol = fgetc(file);
                    headerChecker = false;
                    continue;
                }
                /**
                 * if we write in universum we have to check if we do not write restricted words
                 */
                if (universumCheck) {
                    if (!wordIsNotrestricted(wordBuffer)) {
                        fprintf(stderr, "Universum has restricted word");
                        fclose(file);
                        free(wordBuffer);
                        return false;
                    }
                }
                /**
                 * we copy word from buffer to type-object
                 */
                strcpy(memory->Type[memory->used]->str[memory->Type[memory->used]->elements_used], wordBuffer);
                if (strcmp(wordBuffer, "") != 0) {
                    memory->Type[memory->used]->elements_used++;
                }
                /**
                 * we clear buffer by method strcpy with ""
                 */
                strcpy(wordBuffer, "");
                /**
                 * we resize memory if needed
                 */
                if (memory->Type[memory->used]->elements_used == memory->Type[memory->used]->elements_amount) //str[2][31] -> resize
                    resizeStr(memory->Type[memory->used]);
                wordSize = 0;
            }
            if (symbol == '\n') {
                row++;
                wordSize = 0;
                symbol = fgetc(file);
                if (!headerChecker) {
                    if (universumCheck) {
                        /**
                         * we need to write our word in buffer to our type-object on previous line
                         * so we check to this wasn't universum and we write our word
                         */
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
        /**
         * check that word is less thatn 30
         */
        if (wordSize == MAX_SIZE - 1) {
            fclose(file);
            free(wordBuffer);
            fprintf(stderr, "Error: element couldn't have size more than 30\n");
            return false;
        }
        if (headerChecker) {
            if (headerIsValid(symbol)) {
                /**
                 * we set header and resize memory if needed
                 */
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
        /**
         * here we check that what type of line we gonna get to check
         *      if needed write '(' and ')'
         */
        if (memory->Type[memory->used]->header == 'C') {
            if (commandValueIsValid(symbol)) {
                strncat(wordBuffer, &symbol, 1);
            } else {
                fprintf(stderr,"element %c on line %s has restricted symbol\n", memory->Type[memory->used]->header, memory->Type[memory->used]->row);
                fclose(file); free(wordBuffer);
                return false;
            }
        } else {
            if (valueIsValid(symbol)) {
                strncat(wordBuffer, &symbol, 1);
            } else {
                fprintf(stderr,"element %c on line %s has restricted symbol\n", memory->Type[memory->used]->header, memory->Type[memory->used]->row);
                fclose(file); free(wordBuffer);
                return false;
            }
        }
        headerChecker = false;
    }
    memory->used++;
    fclose(file); free(wordBuffer);
    return true;
}

/**
 * we free memory allocated for Type-object
 *
 * @param Type(type-object)
 */
void freeType(type* Type) {
    for (int i = 0; i < Type->elements_amount; i++) {
        free(Type->str[i]);
    }
    free(Type->row);
    free(Type->str);
    free(Type);
}

/**
 * we free memory allocated for memory-object
 *
 * @param memory (memory-object)
 */
void freeMemory(Memory* memory) {
    for (int i = 0; i < memory->size; i++) {
        freeType(memory->Type[i]);
    }
    free(memory->Type);
    free(memory);
}

/**
 * we free memory allocated for result-object
 *
 * @param res (result-object)
 */
void freeResult(result* res) {
    for (int i = 0; i < res->size; i++) free(res->array[i]);
    free(res->array);
    free(res);
}

/**
 * "createResult" method creates result-object with standard size array[2][x]
 *
 * @return result-object
 */
result* createResult() {
    result* res = malloc(sizeof(result));
    res->size = DEFAULT_SIZE;
    res->used = 0;
    res->array = malloc(sizeof(char*) * DEFAULT_SIZE);
    for (int j = 0; j < DEFAULT_SIZE; j++) res->array[j] = malloc(sizeof(char) * MAX_SIZE);
    return res;
}

/**
 * "resizeResult" method resizes result-object by 1
 *
 * @param res (result-object)
 */
void resizeResult(result* res) {
    res->size++;
    res->array = realloc(res->array, sizeof(char*) * res->size);
    res->array[res->used] = malloc(sizeof(char) * MAX_SIZE);
}

/**
 * "selectByValue" works like "select" method from SQL language
 *      we provide to this method:
 *          @param Memory-object that contains all our data with type-objects
 *          @param result-object that we will fill with our data
 *          @param ch that contains what header we should search for
 *
 *      when we find type-object with header that we need we copy it row to our result-object
 */
void selectByValue(Memory* memory, result* res, char ch) {
    for (int i = 0; i < memory->used; i++) {
        if (memory->Type[i]->header == ch) {
            strcpy(res->array[res->used], memory->Type[i]->row);
            if (++res->used == res->size) resizeResult(res);
        }
    }
}

/**
 * "selectByValue" method works like "select" method from SQL language
 *      we provide to this method:
 *          @param Memory-object that contains all our data with type-objects
 *          @param type-object that we will fill with our data by copyType method
 *          @param line that contains Type-object that we want to copy
 *
 * if we trying to copy N-header object we had to initialize our newType-object
 */
void selectByRow(Memory* memory, type* newType, char* line) {
    bool found = false;
    for (int i = 0; i < memory->used; i++) {
        if (strcmp(memory->Type[i]->row, line) == 0) {
            found = true;
            if (memory->Type[i]->elements_used >= 0) {
                copyType(newType, memory->Type[i]);
            } else {
                newType->header = memory->Type[i]->header;
                newType->elements_amount = DEFAULT_SIZE;
                newType->str = malloc(sizeof(char*) * newType->elements_amount);
                for (int j = 0; j < newType->elements_amount; j++) {
                    newType->str[j] = malloc(sizeof(char) * MAX_SIZE);
                }
                newType->row = malloc(sizeof(char) * MAX_SIZE);
            }
        }
    }
    if (!found) {
        newType->elements_amount = DEFAULT_SIZE;
        newType->str = malloc(sizeof(char*) * newType->elements_amount);
        for (int j = 0; j < newType->elements_amount; j++) {
            newType->str[j] = malloc(sizeof(char) * MAX_SIZE);
        }
        newType->row = malloc(sizeof(char) * MAX_SIZE);
    }
}

/**
 * @param A (type-object)
 * @return
 *      TRUE if set A is empty
 *      FALSE if set A is not
 */
char* empty(type* A) {
    if (A->elements_used == 0) return "true";
    return "false";
}

/**
 * @param A (type-object)
 * @return number of elements in set A
 */
int card(type* A) {
    return A->elements_used;
}

/**
 * "method complement" copy to str elements that are not in set A but in Universum
 * @param A (type-object)
 * @param U (type-object)
 * @param str
 */
void complement(type* A, type* U, char* str) {
    strcpy(str, "S ");
    if (A->header == 'U') {
        return;
    }
    for (int i = 0; i < U->elements_used; i++) {
        bool found = false;
        for (int j = 0; j < A->elements_used; j++) {
            if (strcmp(A->str[j], U->str[i]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            strcat(str, U->str[i]);
            strcat(str, " ");
        }
    }
}

/**
 * "union_" method write to str elements that are in set A or in set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
void union_(type* A, type* B, char* str) {
    strcpy(str, "S ");
    for (int i = 0; i < A->elements_used; i++) {
        strcat(str, A->str[i]);
        strcat(str, " ");
    }
    for (int j = 0; j < B->elements_used; j++) {
        bool found = false;
        for (int i = 0; i < A->elements_used; i++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            strcat(str, B->str[j]);
            strcat(str, " ");
        }
    }
}

/**
 * "intersect" method write to str elements that are in set A and in set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
void intersect(type* A, type* B, char* str) {
    strcpy(str, "S ");
    for (int i = 0; i < A->elements_used; i++) {
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                strcat(str, A->str[i]);
                strcat(str, " ");
                break;
            }
        }
    }
}

/**
 * "minus" method write to str elements that are in set A and are not in set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
void minus(type* A, type* B, char* str) {
    strcpy(str, "S ");
    for (int i = 0; i < A->elements_used; i++) {
        bool found = false;
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            strcat(str, A->str[i]);
            strcat(str, " ");
        }
    }
}

/**
 * "subseteq" method checks if set A is subseteq of set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
char* subseteq(type* A, type* B) {
    for (int i = 0; i < A->elements_used; i++) {
        bool found = false;
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return "false";
    }
    return "true";
}

/**
 * "subset" method checks if set A is subset of set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
char* subset(type* A, type* B) {
    if (A->elements_used == B->elements_used) return "false";
    for (int i = 0; i < A->elements_used; i++) {
        bool found = false;
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return "false";
    }
    return "true";
}

/**
 * "equals" method checks if set A is equal to set B
 *
 * @param A (type-object)
 * @param B (type-object)
 * @param str
 */
char* equals(type* A, type* B) {
    if (A->elements_used != B->elements_used) return "false";
    for (int i = 0; i < A->elements_used; i++) {
        bool found = false;
        for (int j = 0; j < B->elements_used; j++) {
            if (strcmp(A->str[i], B->str[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return "false";
    }
    return "true";
}

/*
 * -------------------RELATIONS-------------------
 */

/**
 * method "getUnion" write to str unqie elements of
 * @param A
 * @param elements
 * @param size
 * @param start
 * @param interval
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

/**
 * "reflexive" method checks if relation R is reflexive
 *
 * @param R (type-object)
 */
char* reflexive(type* R) {
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

/**
 * "symmetric" method checks if relation R is symmetric
 *
 * @param R (type-object)
 */
char* symmetric(type* A) {
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

/**
 * "antisymmetric" method checks if relation R is antisymmetric
 *
 * @param R (type-object)
 */
char* antisymmetric(type* A) {
    for (int i = 0; i < A->elements_used; i+=2) {
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

/**
 * "transitive" method checks if relation R is transitive
 *
 * @param R (type-object)
 */
char* transitive(type* R) {
    for (int i = 0; i < R->elements_used; i+=2) {
        bool transitive = false;
        for (int j = 0; j < R->elements_used; j+=2) {
            if (strcmp(R->str[i+1], R->str[j]) == 0) {
                for (int k = 0; k < R->elements_used; k+=2) {
                    if (strcmp(R->str[k], R->str[i]) == 0 && k != i) {
                        if (strcmp(R->str[k+1], R->str[j+1]) == 0) {
                            transitive = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!transitive) {
            return "false";
        }
    }
    return "true";
}

/**
 * "function" method checks if relation R is function
 *
 * @param R (type-object)
 */
char* function(type* A) {
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

/**
 * "domain" write domain of relation R
 *
 * @param R (type-object)
 */
void domain(type* R, char* str) {
    strcpy(str, "S ");
    int size = 0;
    char elements[R->elements_used][31];
    getUnique(R, elements, &size, 0, 2);
    for (int i = 0; i < size; i++) {
        strcat(str, elements[i]);
        if (strcmp(elements[i], "") == 0) {
            continue;
        }
        strcat(str, " ");
    }
}

/**
 * "codomain" write codomain of relation R
 *
 * @param R (type-object)
 */
void codomain(type* R, char* str) {
    strcpy(str, "S ");
    int size = 0;
    char elements[R->elements_used][31];
    getUnique(R, elements, &size, 1, 2);
    for (int i = 0; i < size; i++) {
        strcat(str, elements[i]);
        if (strcmp(elements[i], "") == 0) {
            continue;
        }
        strcat(str, " ");
    }
}

/**
 * "injective" method checks if relation R is injective for given sets
 *
 * @param R (type-object)
 */
char* injective(type* R, type* A, type* B) {
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

/**
 * "surjective" method checks if relation R is injective for given sets
 *
 * @param R (type-object)
 */
char* surjective(type* R, type* A, type* B) {
    if (B->header != 'S') {
        return "false";
    }
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

/**
 * "bijective" method checks if relation R is injective for given sets
 *
 * @param R (type-object)
 */
char* bijective(type* R, type* A, type* B) {
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

/**
 * "checkForRelationAndSetElementsInUniversum" method checks if relations and sets are correct and contains only
 * elements present in universum
 *
 * @param memory (Memory-object)
 * @return
 *      True if all relations and sets are correct and contains only elements present in universum
 *      False if one of relations or sets is incorrect or contains elements not present in universum
 */
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

/**
 * checks if provided type-object has not providedheader
 * @param A (type-object)
 * @param header
 * @return
 *      True if type-object has not provided header
 *      False if type-object has provided header
 */
bool headerIsNotEqual(type* A, char header) {
    if (A->header == header) return false;
    return true;
}

/**
 * "callFunctionByItName" method calls function by given in C lines function-names
 *      we provide variables:
 *          @param name  - name of function
 *          @param executors - memory-object that has all types that was provided in C lines
 *          @param U    - universum
 *          @param str - string that is used as buffer to get response from function and provide it to types that
 *                  lays in Memory-object memory in main(currently in execution function)
 *
 *      than we separated function by amount of arguments they use:
 *          1 arguement - empty, card...
 *          2 arguements - union, intersect...
 *          3 arguements - injective, bijective, surjective.
 *              in case we didnt find function we return error that not right amount of arguments were provided
 * @return
 */
bool callFunctionByItName(char* name, Memory* executors, type* U, char* str) {
    if (executors->used == 1) {
        if (strcmp(name, "empty") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S')
            && headerIsNotEqual(executors->Type[0], 'U')) {
                fprintf(stderr, "Error: empty() can be used only with sets\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, empty(executors->Type[0])); return true;
        }
        if (strcmp(name, "card") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S')
            && headerIsNotEqual(executors->Type[0], 'U')) {
                fprintf(stderr, "Error: card() can be used only with sets\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            sprintf(str, "%d", card(executors->Type[0])); return true;
        }
        if (strcmp(name, "complement") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') && headerIsNotEqual(executors->Type[0], 'U')) {
                fprintf(stderr, "Error: complement() can be used only with sets and universium\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            complement(executors->Type[0], U, str); return true;
        }
        if (strcmp(name, "reflexive") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: reflexive() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, reflexive(executors->Type[0])); return true;
        }
        if (strcmp(name, "symmetric") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: symmetric() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, symmetric(executors->Type[0]));
            return true;
        }
        if (strcmp(name, "antisymmetric") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: antisymmetric() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, antisymmetric(executors->Type[0])); return true;
        }
        if (strcmp(name, "transitive") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: transitive() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, transitive(executors->Type[0])); return true;
        }
        if (strcmp(name, "function") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: function() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, function(executors->Type[0])); return true;
        }
        if (strcmp(name, "domain") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: domain() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            domain(executors->Type[0], str); return true;
        }
        if (strcmp(name, "codomain") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R')) {
                fprintf(stderr, "Error: codomain() can be used only with relations\n");
                strcpy(str, "FAIL\n");
                return false;
            }
            codomain(executors->Type[0], str); return true;
        }
    }

    if (executors->used == 2) {
        if (strcmp(name, "union") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: union() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            union_(executors->Type[0], executors->Type[1], str); return true;
        }
        if (strcmp(name, "intersect") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: intersect() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            intersect(executors->Type[0], executors->Type[1], str); return true;
        }
        if (strcmp(name, "minus") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: minus() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            minus(executors->Type[0], executors->Type[1], str); return true;
        }
        if (strcmp(name, "subseteq") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: subseteq() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, subseteq(executors->Type[0], executors->Type[1])); return true;
        }
        if (strcmp(name, "subset") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: subset() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, subset(executors->Type[0], executors->Type[1])); return true;
        }
        if (strcmp(name, "equals") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'S') || headerIsNotEqual(executors->Type[1], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with sets");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, equals(executors->Type[0], executors->Type[1])); return true;
        }
    }

    if (executors->used == 3) {
        if (strcmp(name, "injective") == 0)
        {
            if (headerIsNotEqual(executors->Type[0], 'R') || headerIsNotEqual(executors->Type[1], 'S')
                || headerIsNotEqual(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, injective(executors->Type[0], executors->Type[1], executors->Type[2])); return true;
        }
        if (strcmp(name, "surjective") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R') || headerIsNotEqual(executors->Type[1], 'S')
                || headerIsNotEqual(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, surjective(executors->Type[0], executors->Type[1], executors->Type[2]));
            return true;
        }
        if (strcmp(name, "bijective") == 0) {
            if (headerIsNotEqual(executors->Type[0], 'R') || headerIsNotEqual(executors->Type[1], 'S')
                || headerIsNotEqual(executors->Type[2], 'S')) {
                fprintf(stderr, "Error: equals() can be used only with R S S format");
                strcpy(str, "FAIL\n");
                return false;
            }
            strcpy(str, bijective(executors->Type[0], executors->Type[1], executors->Type[2]));
            return true;
        }
    }
    fprintf(stderr, "Error: not right function atributes");
    return false;
}

/**
 * "getMaxLength" returns the length of the longest string in the array of type-objects
 * @param memory
 * @return
 */
int getMaxLength(Memory* memory) {
    int max = 0;
    for (int i = 0; i < memory->used; i++) {
        if (memory->Type[i]->elements_used > max) {
            max = memory->Type[i]->elements_used;
        }
    }
    return max;
}

/**
 * "initExecutiveToEnd" initializes the executive to the end
 *      method was created because in use of the "executors" variable we had to copy our elements
 *      and as standard we have array[2][x] but ininialized only first element
 *      so to free our memory we had to allocate it to the end
 * @param executive (Memory-object)
 */
void initExecutiveToEnd(Memory* executive) {
    executive->Type[executive->used]->row = malloc(sizeof(char) * 3);
    executive->Type[executive->used]->str = malloc(sizeof(char*) * executive->Type[executive->used]->elements_used);
    for (int j = 0; j < executive->Type[executive->used]->elements_used; j++) {
        executive->Type[executive->used]->str[j] = malloc(sizeof(char) * MAX_SIZE);
    }
}

/**
 * "executeFunction" method gets all C-lines and executes them by getting and providing
 *      types and function name to the "callFunctionByItName" method
 * @param memory (Memory-object)
 * @return
 *      True if all lines were executed successfully
 *      False if some line was not executed successfully
 */
bool executeFunction(Memory* memory) {
    int size = getMaxLength(memory);
    result* commands = createResult();
    selectByValue(memory, commands, 'C');
    char* str = malloc(sizeof(char) * (size * MAX_SIZE));
    for (int i = 0; i < commands->used; i++) {
        Memory* executive = malloc(sizeof(Memory));
        executive->size = DEFAULT_SIZE;
        executive->Type = malloc(sizeof(type*) * executive->size);
        for (int j = 0; j < executive->size; j++) {
            executive->Type[j] = malloc(sizeof(type));
        }
        executive->used = 0;

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
                for (int j = 0; j < executive->size; j++) {
                    free(executive->Type[j]);
                }
                free(executive->Type);
                free(executive);
                free(str);
                freeResult(commands);
                freeType(command);
                return false;
                break;
            }
            copyType(executive->Type[executive->used], Type);
            if (++executive->used == executive->size) {
                executive->size++;
                executive->Type = realloc(executive->Type, sizeof(type*) * memory->size);
                executive->Type[executive->used] = malloc(sizeof(type));
            }
            freeType(Type);
        }
        result* Universum = createResult();
        selectByValue(memory, Universum, 'U');
        type* U = malloc(sizeof(type));
        selectByRow(memory, U, Universum->array[0]);
        if (!callFunctionByItName(command->str[0], executive, U, str)) {
            freeType(U);
            freeType(command);
            freeResult(Universum);
            initExecutiveToEnd(executive);
            freeMemory(executive);
            freeResult(commands);
            return false;
        }
        for (int j = 0; j < memory->used; j++) {
            if (strcmp(memory->Type[j]->row, commands->array[i]) == 0) {
                memory->Type[j]->header = 'F';
                for (int k = 0; k < memory->Type[j]->elements_amount; k++) {
                    memory->Type[j]->str[k] = realloc(memory->Type[j]->str[k], sizeof(char) * size * MAX_SIZE);
                }
                strcpy(memory->Type[j]->str[0], str);
            }
        }
        freeType(U);
        freeType(command);
        freeResult(Universum);
        initExecutiveToEnd(executive);
        freeMemory(executive);
    }
    free(str);
    freeResult(commands);
    return true;
}
/**
 * "hasDuplicateRelation" method checks if the relation has duplicate
 * @param memory
 * @return
 *      True if the relation has no duplicate
 *      False if the relation has duplicate
 */
bool hasDuplicateRelation(Memory* memory) {
    result * relations = createResult();
    selectByValue(memory, relations, 'R');
    for (int i = 0; i < relations->used; i++) {
        type * relation = malloc(sizeof(type));
        selectByRow(memory, relation, relations->array[i]);
        for (int j = 0; j < relation->elements_used; j+=2) {
            for (int k = 0; k < relation->elements_used; k+=2) {
                if (j == k) {
                    continue;
                }
                if (strcmp(relation->str[j], relation->str[k]) == 0) {
                    if (strcmp(relation->str[j + 1], relation->str[k + 1]) == 0) {
                        fprintf(stderr, "Relation has duplacates");
                        freeType(relation);
                        freeResult(relations);
                        return false;
                    }
                }
            }
        }
        freeType(relation);
    }
    freeResult(relations);
    return true;
}

/**
 * "checkAndRefactorRelations" method check if relation have right amount of elements and refactor them:
 *      method deleted '(' and ')' so we can use them in the future
 * @param memory
 * @return
 *      True if the relation has no duplicate
 *      False if the relation has duplicate
 */
bool checkAndRefactorRelations(Memory* memory) {
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
    if (!hasDuplicateRelation(memory)) {
        return false;
    }
    return true;
}

/**
 * check for duplicate in the all objects in our Memory-object
 *
 * @param memory
 * @return
 *      True if type-object has no duplicate
 *      False if type-object has duplicate
 */
bool checkForDuplicates(Memory* memory) {
    result* universum = createResult();
    selectByValue(memory, universum, 'U');
    if (universum->used > 1) {
        fprintf(stderr, "Universum have more than one row\n");
        freeResult(universum);
        return false;
    }
    type* U = malloc(sizeof(type));
    selectByRow(memory, U, universum->array[0]);
    for (int i = 0; i < U->elements_used; i++) {
        bool duplicate = false;
        for (int j = 0; j < U->elements_used; j++) {
            if (strcmp(U->str[i], U->str[j]) == 0 && i != j) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            fprintf(stderr, "Universum have duplicate elements\n");
            freeType(U);
            freeResult(universum);
            return false;
        }
    }
    freeType(U);
    freeResult(universum);
//------------------------------------------------------------------------------------------------------------------
    result* sets = createResult();
    selectByValue(memory, sets, 'S');
    for (int i = 0; i < sets->used; i++) {
        type* set = malloc(sizeof(type));
        selectByRow(memory, set, sets->array[i]);
        for (int j = 0; j < set->elements_used; j++) {
            bool duplicate = false;
            for (int k = 0; k < set->elements_used; k++) {
                if (strcmp(set->str[j], set->str[k]) == 0 && j != k) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                fprintf(stderr, "Set have duplicate elements\n");
                freeType(set);
                freeResult(sets);
                return false;
            }
        }
        freeType(set);
    }
    freeResult(sets);
    return true;
}

int main(int argc, char **argv) {
    (void)argc;
    if (argv[1] == NULL) {
        fprintf(stderr, "No file name\n");
        return 1;
    }
    if (argv[2] != NULL) {
        fprintf(stderr, "Too much program argumetns\n");
        return 1;
    }
    Memory *memory = createMemory();
    if (!readFromFileV2(argv[1], memory)) {
        freeMemory(memory);
        return 1;
    }
    if (!checkForDuplicates(memory)) {
        freeMemory(memory);
        return 1;
    }
    if (!checkAndRefactorRelations(memory)) {
        freeMemory(memory);
        return 1;
    }
    result* res = createResult();
    if (!checkForRelationAndSetElementsInUniversum(memory)) {
        fprintf(stderr, "ERROR: set/relation have element that is not in universum\n");
        freeMemory(memory);
        freeResult(res);
        return 1;
    }
    if (!executeFunction(memory)) {
        freeMemory(memory);
        freeResult(res);
        return 1;
    }
    int cursor = 1;
    for (int i = 0; i < memory->used; i++) dump(memory->Type[i], &cursor);
    freeResult(res);
    freeMemory(memory);
    printf("\n");
    return 0;
}