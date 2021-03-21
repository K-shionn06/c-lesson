#include <stdio.h>

#define STACK_SIZE 16

typedef enum {
    TRUE = 1,
    FALSE = 0,
} BOOL;

typedef long data_t;

typedef struct Stack {
    unsigned int pos;
    data_t body[STACK_SIZE];
} stack_t;

/* Stack suite core functions */

BOOL stack_pop(stack_t *, data_t *);
BOOL stack_push(stack_t *, data_t);

BOOL stack_isfull(stack_t *);
BOOL stack_isempty(stack_t *);

void stack_clear(stack_t *);
void stack_print_all(stack_t *);

/* Unit tests for Stack suite functions */

void unit_test_suite();

void print_stack_status();

void test_stack_clear();

void test_stack_push_123();
void test_stack_push_when_full();

void test_stack_pop_when_empty();

void test_stack_push_pop_123();
void test_stack_push_pop_123_456();
