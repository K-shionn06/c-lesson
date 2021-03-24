#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define STACK_SIZE 1024

typedef struct {
    unsigned int pos;
    stack_data_t body[STACK_SIZE];
} stack_t;


stack_t global_stack;


/* core functions for global_stack */


void stack_clear() {
    for (int i = 0; i < STACK_SIZE; i++) {
        global_stack.body[i].dtype = S_NUMBER;
        global_stack.body[i].u.number = 0;
    }
    global_stack.pos = 0;
}

void stack_print_all() {
    int i;
    puts("-----------------------------");
    for (i = STACK_SIZE-1; i >= 0; i--) {
        switch (global_stack.body[i].dtype) {
            case S_NUMBER:
                printf("     | %014d |\n", global_stack.body[i].u.number);
                break;
            case S_EXE_NAME:
            case S_LIT_NAME:
                printf("     | %p | -> %s\n", global_stack.body[i].u.name,
                                                global_stack.body[i].u.name);
                break;
        }
        printf("%04d |----------------|\n", i);
    }
    puts("-----------------------------");
}

bool stack_isfull() {
    if (STACK_SIZE == global_stack.pos)
        return true;
    else
        return false;
}

bool stack_isempty() {
    if (0 == global_stack.pos)
        return true;
    else
        return false;
}

void stack_push_number(int data) {
    assert( !stack_isfull() );

    global_stack.body[global_stack.pos].dtype = S_NUMBER;
    global_stack.body[global_stack.pos].u.number = data;
    global_stack.pos += 1;
}

void stack_push_exe_name(char *data) {
    assert( !stack_isfull() );

    global_stack.body[global_stack.pos].dtype = S_EXE_NAME;
    global_stack.body[global_stack.pos].u.name = data;
    global_stack.pos += 1;
}

void stack_push_lit_name(char *data) {
    assert( !stack_isfull() );

    global_stack.body[global_stack.pos].dtype = S_LIT_NAME;
    global_stack.body[global_stack.pos].u.name = data;
    global_stack.pos += 1;
}

void stack_pop(stack_data_t *out_data) {
    assert( !stack_isempty() );

    global_stack.pos -= 1;
    out_data->dtype = global_stack.body[global_stack.pos].dtype;

    switch (out_data->dtype) {
        case S_NUMBER:
            out_data->u.number = global_stack.body[global_stack.pos].u.number;
            break;
        case S_EXE_NAME:
        case S_LIT_NAME:
            out_data->u.name = global_stack.body[global_stack.pos].u.name;
            break;
    }
}

void stack_pop_number(int *out_number) {
    stack_data_t data;
    stack_pop(&data);

    assert(S_NUMBER == data.dtype);

    *out_number = data.u.number;
}

void stack_pop_exe_name(char **out_str) {
    stack_data_t data;
    stack_pop(&data);

    assert(S_EXE_NAME == data.dtype);

    *out_str = data.u.name;
}

void stack_pop_lit_name(char **out_str) {
    stack_data_t data;
    stack_pop(&data);

    assert(S_LIT_NAME == data.dtype);

    *out_str = data.u.name;
}


/* unit tests */


void test_stack_push_number_123() {
    int input = 123;

    stack_clear();
    stack_push_number(input);

    assert(S_NUMBER == global_stack.body[0].dtype);
    assert(input == global_stack.body[0].u.number);
}

void test_stack_push_string_as_exe() {
    char *input = "helloworld";

    stack_clear();
    stack_push_exe_name(input);

    assert(S_EXE_NAME == global_stack.body[0].dtype);
    assert(0 == strcmp(input, global_stack.body[0].u.name));
}

void test_stack_push_string_as_lit() {
    char *input = "helloworld";

    stack_clear();
    stack_push_lit_name(input);

    assert(S_LIT_NAME == global_stack.body[0].dtype);
    assert(0 == strcmp(input, global_stack.body[0].u.name));
}

void test_stack_pop_when_empty() {
    // This should fail

    stack_data_t data;

    stack_clear();
    stack_pop(&data);
}

void test_stack_pop_123() {
    int input = 123, actual;

    stack_clear();
    stack_push_number(input);
    stack_pop_number(&actual);

    assert(input == actual);
}

void test_stack_isfull() {
    int input = 123;
    bool actual;

    stack_clear();
    for (int i = 0; i < STACK_SIZE; i++) {
        stack_push_number(input);
    }
    actual = stack_isfull();

    assert(true == actual);
}

void test_stack_isempty() {
    bool actual;

    stack_clear();
    actual = stack_isempty();

    assert(true == actual);
}

void test_suite() {
    test_stack_push_number_123();
    // test_stack_pop_when_empty();
    test_stack_pop_123();
    test_stack_isempty();
    test_stack_isfull();
}

int main() {
    // test_suite();
    test_stack_push_string_as_exe();
    test_stack_push_string_as_lit();
    return 0;
}
