#include "continuation.h"
#include "exec_array.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


#define STACK_SIZE 1024

struct CoStack {
    unsigned int pos;
    struct Continuation array[STACK_SIZE];
};


struct CoStack co_stack;


/* functions */


static bool co_stack_isempty() {
    if (0 == co_stack.pos)
        return 1;
    else
        return 0;
}

static bool co_stack_isfull() {
    if (STACK_SIZE == co_stack.pos)
        return 1;
    else
        return 0;
}

static void co_stack_clear() {
    for (int i = 0; i < STACK_SIZE; i++) {
        co_stack.array[i].pc = 0;
        co_stack.array[i].byte_codes = NULL;
    }
    co_stack.pos = 0;
}

void co_stack_push(int pc, struct EA_ElementArray* byte_codes) {
    assert( !co_stack_isfull() );

    co_stack.array[co_stack.pos].pc = pc;
    co_stack.array[co_stack.pos].byte_codes = byte_codes;
    co_stack.pos++;
}

void co_stack_pop(struct Continuation* out_co) {
    assert( !co_stack_isempty() );

    co_stack.pos--;
    out_co->pc = co_stack.array[co_stack.pos].pc;
    out_co->byte_codes = co_stack.array[co_stack.pos].byte_codes;
}

void test_co_stack_push_pop() {
    struct EA_Element input_elem;
    call_compile_exec_array("1 2 3 add sum}", &input_elem);

    co_stack_push(2, input_elem.u.byte_codes);

    struct Continuation actual_co;
    co_stack_pop(&actual_co);

    assert( is_same_byte_codes(input_elem.u.byte_codes, actual_co.byte_codes) );
}

int main() {
    test_co_stack_push_pop();

    return 0;
}
