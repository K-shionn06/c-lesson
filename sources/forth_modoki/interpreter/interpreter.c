#include "clesson.h"
#include "stack.h"
#include "eval.h"
#include <stdio.h>
#include <assert.h>


static void init_interpreter() {
    register_primitives();
    stack_clear();
}


/* unit tests */


static void eval_with_file(char* file_name) {
    cl_getc_attach_file(file_name);
    eval();
    cl_getc_detach_file();
}

static void test_ps_repeat() {
    char* input = "100 4 {10 add} repeat";
    int expect = 140;

    init_interpreter();

    eval_with_file("ps_prog/repeat.ps");

    cl_getc_set_src(input);
    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_ps_sam_k() {
    char* input = "10 sum_k";
    int expect = 55;

    init_interpreter();

    eval_with_file("ps_prog/sum_k.ps");

    cl_getc_set_src(input);
    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_ps_sam_k_2() {
    char* input = "4 sum_k_2";
    int expect = 30;

    init_interpreter();

    eval_with_file("ps_prog/sum_k_2.ps");

    cl_getc_set_src(input);
    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_suite() {
    test_ps_repeat();
    test_ps_sam_k();
    test_ps_sam_k_2();
}

int main() {
    test_suite();

    return 0;
}
