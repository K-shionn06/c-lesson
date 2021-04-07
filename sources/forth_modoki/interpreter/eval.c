#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "dict.h"
#include "exec_array.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>


/* functions */


static void eval_exec_array(struct EA_ElementArray *byte_codes) {
    struct EA_Element* byte_code;
    struct D_Element* d_elem;

    for (int i = 0; i < byte_codes->len; ++i) {
        byte_code = &byte_codes->elements[i];

        switch (byte_code->dtype) {
            default:
                assert(false);

            case EA_NUMBER:
                stack_push_number(byte_code->u.number);
                break;

            case EA_LIT_NAME:
                stack_push_lit_name(byte_code->u.name);
                break;

            case EA_EXE_NAME:
                assert( dict_key_isused(byte_code->u.name) );

                d_elem = dict_get(byte_code->u.name);
                switch (d_elem->dtype) {
                    default:
                        assert(false);

                    case D_NUMBER:
                        stack_push_number(d_elem->u.number);
                        break;

                    case D_C_FUNC:
                        d_elem->u.cfunc();
                        break;

                    case D_BYTE_CODES:
                        eval_exec_array(d_elem->u.byte_codes);
                        break;
                }
                break;
        }
    }
}

static void eval() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };
    struct D_Element* d_elem;
    struct EA_Element ea_elem;

    do {
        ch = parse_one(ch, &token);
        assert(UNKNOWN != token.ltype);

        switch (token.ltype) {
            default:
                assert(false);

            case SPACE:
                break;

            case NUMBER:
                stack_push_number(token.u.number);
                break;

            case LITERAL_NAME:
                stack_push_lit_name(token.u.name);
                break;

            case OPEN_CURLY:
                ch = compile_exec_array(ch, &ea_elem);
                stack_push_byte_codes(ea_elem.u.byte_codes);
                break;

            case EXECUTABLE_NAME:
                assert( dict_key_isused(token.u.name) );

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

                    case D_BYTE_CODES:
                        eval_exec_array(d_elem->u.byte_codes);
                        break;
                }
                break;
        }
    } while (EOF != ch);
}

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

static void execute_ifelse() {
    struct EA_ElementArray* byte_codes2 = stack_pop_byte_codes();
    struct EA_ElementArray* byte_codes1 = stack_pop_byte_codes();
    int boolean = stack_pop_number();

    assert((1 == boolean) || (0 == boolean));

    if(boolean)
        eval_exec_array(byte_codes1);
    else
        eval_exec_array(byte_codes2);
}

static void register_primitives() {
    dict_put_cfunc("add", execute_add);
    dict_put_cfunc("def", execute_def);
    dict_put_cfunc("ifelse", execute_ifelse);
}

/* Unit tests */


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

static void test_eval_exec_array() {
    char *input = "/abc {1 2} def abc add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_exec_array_complex() {
    char* input = 
        "/ZZ {6} def \
         /YY {ZZ 5 add} def \
         /ZZYYadd {ZZ YY add} def \
         ZZYYadd";
    int expect = 17;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_ifelse_true() {
    char* input = "20 1 {30 add} {20 add} ifelse";
    int expect = 50;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_ifelse_false() {
    char* input = "20 0 {30 add} {20 add} ifelse";
    int expect = 40;

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

    test_eval_exec_array();
    test_eval_exec_array_complex();

    test_eval_ifelse_true();
    test_eval_ifelse_false();

    return 0;
}
