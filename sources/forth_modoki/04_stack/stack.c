#include "stack.h"
#include "assert.h"
#include <stdio.h>

stack_t global_stack;

int main() {
    unit_test_suite(&global_stack);

/* Stack system core functions */

BOOL stack_pop(stack_t *stack, data_t * popped_data) {
    if (FALSE == stack_isempty(stack)) {
        *popped_data = stack->body[--stack->pos];
        stack->body[stack->pos] = 0;
        return TRUE;
    }
    else {
        return FALSE;
    }
};

BOOL stack_push(stack_t *stack, data_t data) {
    if (FALSE == stack_isfull(stack)) {
        stack->body[stack->pos++] = data;
        return TRUE;
    }
    else {
        return FALSE;
    }
};

BOOL stack_isfull(stack_t *stack) {
    if (STACK_SIZE == stack->pos)
        return TRUE;
    else
        return FALSE;
}

BOOL stack_isempty(stack_t *stack) {
    if (0 == stack->pos)
        return TRUE;
    else
        return FALSE;
}

void stack_clear(stack_t *stack) {
    for (int i = 0; i < STACK_SIZE; i++) {
        stack->body[i] = 0;
    }
    stack->pos = 0;
};

void stack_print_all(stack_t *stack) {
    int i;
    puts("-----------------------------");
    for (i = STACK_SIZE-1; i >= 0; i--) {
        printf("     | %014ld |\n", stack->body[i]);
        printf("%04d |----------------|\n", i);
    }
    puts("-----------------------------");
}

/* Unit tests for Stack system functions */

void assert_stack_push_TRUE(stack_t *stack, data_t input) {
    assert(TRUE == stack_push(stack, input));
}

void assert_stack_push_FALSE(stack_t *stack, data_t input) {
    assert(FALSE == stack_push(stack, input));
}

void assert_stack_pop_TURE(stack_t *stack, data_t *popped_data) {
    assert(TRUE == stack_pop(stack, popped_data));
}
void assert_stack_pop_FALSE(stack_t *stack, data_t *popped_data) {
    assert(FALSE == stack_pop(stack, popped_data));
}

void unit_test_suite(stack_t *stack) {
    /* Run this function to test functions */

    test_stack_clear(stack);

    test_stack_push_123(stack);
    test_stack_push_maximum(stack);
    test_stack_push_when_full(stack);

    test_stack_pop_when_empty(stack);

    test_stack_push_pop_123(stack);
    test_stack_push_pop_123_456(stack);
    test_stack_push_pop_string(stack);
}

void test_stack_clear(stack_t *stack) {
    stack_push(stack, 123);
    stack_push(stack, 456);
    stack_push(stack, 789);

    stack_clear(stack);

    for (int i = 0; i < STACK_SIZE; i++) {
        assert(0 == stack->body[i]);
    }
    assert(TRUE == stack_isempty(stack));
    assert(0 == stack->pos);
}

void test_stack_push_123(stack_t *stack) {
    stack_clear(stack);

    assert_stack_push_TRUE(stack, 123);
}

void test_stack_push_maximum(stack_t *stack) {
    int count = 0;
    long input = 123;

    stack_clear(stack);

    while (1) {
        if ((STACK_SIZE == count) && (TRUE == stack_isfull(stack))) {
            assert_stack_push_FALSE(stack, input);
            break;
        }
        else {
            assert_stack_push_TRUE(stack, input);
        }
        count++;
    }
}

void test_stack_push_when_full(stack_t *stack) {
    stack_clear(stack);

    for (int i = 0; i < STACK_SIZE; i++)
        stack_push(stack, 100+i);

    assert_stack_push_FALSE(stack, 123);
}

void test_stack_pop_when_empty(stack_t *stack) {
    data_t popped_data;

    stack_clear(stack);

    assert_stack_pop_FALSE(stack, &popped_data);
}

void test_stack_push_pop_123(stack_t *stack) {
    long input = 123;
    data_t popped_data;
    
    stack_clear(stack);

    assert_stack_push_TRUE(stack, input);
    assert_stack_pop_TURE(stack, &popped_data);

    assert(input == popped_data);
}

void test_stack_push_pop_123_456(stack_t *stack) {
    long input1 = 123, input2 = 456;
    data_t popped_data;

    stack_clear(stack);

    assert_stack_push_TRUE(stack, input1);
    stack_print_all(stack);
    assert_stack_push_TRUE(stack, input2);
    stack_print_all(stack);

    assert_stack_pop_TURE(stack, &popped_data);
    stack_print_all(stack);
    assert(input2 == popped_data);
    assert_stack_pop_TURE(stack, &popped_data);
    stack_print_all(stack);
    assert(input1 == popped_data);
}

void test_stack_push_pop_string(stack_t *stack) {
    char *input = "hello world";
    data_t popped_data;

    stack_clear(stack);

    assert_stack_push_TRUE(stack, (long)input);
    assert_stack_pop_TURE(stack, &popped_data);

    assert(0 == strcmp(input, (char *)popped_data));
}
