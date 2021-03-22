#include <stdio.h>
#include <stdbool.h>

#define STACK_SIZE 16

enum StackDataType {
    NUMBER,
    NAME
};

union StackData {
    int number;
    char *name;
};

typedef struct {
    enum StackDataType dtype;
    union StackData u;
} stack_data_t;

typedef struct {
    unsigned int pos;
    stack_data_t body[STACK_SIZE];
} stack_t;

bool stack_push_number(int);
bool stack_push_string(char *);
bool stack_pop(stack_data_t *);

void stack_clear();
