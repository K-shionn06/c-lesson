#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define STACK_SIZE 16

typedef struct {
    unsigned int pos;
    stack_data_t body[STACK_SIZE];
} stack_t;


stack_t global_stack;


/* core functions for global_stack */

bool stack_push_number(int data) {
    if (false == stack_isfull()) {
        global_stack.body[global_stack.pos].dtype = NUMBER;
        global_stack.body[global_stack.pos].u.number = data;
        global_stack.pos += 1;
        return true;
    }
    else {
        return false;
    }
}

bool stack_push_string(char *data) {
    if (false == stack_isfull()) {
        global_stack.body[global_stack.pos].dtype = NAME;
        global_stack.body[global_stack.pos].u.name = data;
        global_stack.pos += 1;
        return true;
    }
    else {
        return false;
    }
}

bool stack_pop(stack_data_t *out_stack_data) {
    if (false == stack_isempty()) {
        global_stack.pos -= 1;
        out_stack_data->dtype = global_stack.body[global_stack.pos].dtype;
        switch (out_stack_data->dtype) {
            case NUMBER:
                out_stack_data->u.number = global_stack.body[global_stack.pos].u.number;
                break;
            case NAME:
                out_stack_data->u.name = global_stack.body[global_stack.pos].u.name;
                break;
        }
        return true;
    }
    else {
        return false;
    }
}

/* unit tests */

void test_stack_push_number_123() {
    int input = 123;
    bool actual;

    stack_clear();
    actual = stack_push_number(input);

    assert(NUMBER == global_stack.body[0].dtype);
    assert(input == global_stack.body[0].u.number);
    assert(actual);
}

void test_stack_push_string_helloworld() {
    char *input = (char *)malloc(strlen("helloworld") + 1);
    strcpy(input, "helloworld");
    bool actual;

    stack_clear();
    actual = stack_push_string(input);

    assert(NAME == global_stack.body[0].dtype);
    assert(input == global_stack.body[0].u.name);
    assert(actual);
}

void test_stack_pop_when_empty() {
    stack_data_t out_stack_data;
    bool actual;

    stack_clear();
    actual = stack_pop(&out_stack_data);

    assert(false == actual);
}

void test_stack_pop_123() {
    int input = 123;
    stack_data_t out_stack_data;
    bool actual;

    stack_clear();
    stack_push_number(input);
    actual = stack_pop(&out_stack_data);

    assert(NUMBER == out_stack_data.dtype);
    assert(input == out_stack_data.u.number);
    assert(actual);
}
void test_stack_pop_helloworld() {
    char *input = (char *)malloc(strlen("helloworld") + 1);
    strcpy(input, "helloworld");
    stack_data_t out_stack_data;
    bool actual;

    stack_clear();
    stack_push_string(input);
    actual = stack_pop(&out_stack_data);

    assert(NAME == out_stack_data.dtype);
    assert(input == out_stack_data.u.name);
    assert(actual);
}

void test_suite() {
    test_stack_push_number_123();
    test_stack_push_string_helloworld();
    test_stack_pop_when_empty();
    test_stack_pop_123();
    test_stack_pop_helloworld();
}

int main() {
    test_suite();
}