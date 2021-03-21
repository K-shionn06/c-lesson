#include "stack.h"
#include "assert.h"
#include <stdio.h>

stack_t global_stack;

int main() {
    return 0;
}

/* Stack suite core functions */

BOOL stack_pop(stack_t *stack, data_t * popped_data) {
    if (FALSE == stack_isempty(stack)) {
        *popped_data = stack->body[--stack->pos];
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
