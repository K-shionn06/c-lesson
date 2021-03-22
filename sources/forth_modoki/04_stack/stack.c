#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


stack_t global_stack;


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
