#include "stack.h"
#include "assert.h"
#include <stdio.h>

stack_t global_stack;

int main() {
    printf("%d, %ld\n", global_stack.pos, global_stack.body[4]);
    return 0;
}


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
