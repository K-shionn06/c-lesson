#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "dict.h"
#include "exec_array.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>


static void execute_add() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number1 + number2);
}

static void execute_def() {
    struct S_Element s_elem;
    stack_pop(&s_elem);

    char *lit_name = stack_pop_lit_name();

    switch (s_elem.dtype) {
        default:
            assert(false);

        case S_NUMBER:
            dict_put_number(lit_name, s_elem.u.number);
            break;

        case S_BYTE_CODES:
            dict_put_byte_codes(lit_name, s_elem.u.byte_codes);
            break;
    }
}

static void register_primitives() {
    dict_put_cfunc("add", execute_add);
    dict_put_cfunc("def", execute_def);
}

void eval() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };
    struct D_Element* d_elem;
    struct EA_Element ea_elem;

    do {
        ch = parse_one(ch, &token);
        if (UNKNOWN!= token.ltype) {
            switch (token.ltype) {
                default:
                    assert(false);

                case SPACE:
                    break;

                case NUMBER:
                    stack_push_number(token.u.number);
                    break;

                case EXECUTABLE_NAME:
                    if ( !dict_key_isused(token.u.name) )
                        assert(false);

                    d_elem = dict_get(token.u.name);
                    switch (d_elem->dtype) {
                        default:
                            assert(false);

                        case D_NUMBER:
                            stack_push_number(d_elem->u.number);
                            break;

                        case D_C_FUNC:
                            d_elem->u.cfunc();
                            break;
                    }
                    break;

                case LITERAL_NAME:
                    stack_push_lit_name(token.u.name);
                    break;

                case OPEN_CURLY:
                    compile_exec_array(&ea_elem);
                    stack_push_byte_codes(ea_elem.u.byte_codes);
            }
        }
    } while (EOF != ch);
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);

}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}


static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_lit() {
    char *input = "/helloworld";
    char *expect = "helloworld";

    cl_getc_set_src(input);

    eval();

    char *actual = stack_pop_lit_name();

    assert(0 == strcmp(expect, actual));
}

static void test_eval_def() {
    char *input = "/abc 123 def /efg 456 def abc efg add";
    int expect = 579;
    
    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

int main() {
    register_primitives();
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_lit();
    test_eval_def();

    return 0;
}
