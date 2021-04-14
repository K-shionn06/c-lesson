#include "clesson.h"
#include "stack.h"
#include "eval.h"
#include <stdio.h>

void init_interpreter() {
    register_primitives();
    stack_clear();
}

int main() {
    init_interpreter();
    cl_getc_attach_file("sample.ps");

    eval();

    printf("Stack top: %d\n", stack_pop_number());
    return 0;
}
