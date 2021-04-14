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

static void execute_sub() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number2 - number1);
}

static void execute_div() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number2 / number1);
}

static void execute_mul() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number1 * number2);
}

static void execute_eq() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number1 == number2)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_neq() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number1 != number2)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_gt() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number2 > number1)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_ge() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number2 >= number1)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_lt() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number2 < number1)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_le() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    if (number2 <= number1)
        stack_push_number(1);
    else
        stack_push_number(0);
}

static void execute_pop() {
    stack_pop_number();
}

static void execute_exch() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number1);
    stack_push_number(number2);
}

static void execute_dup() {
    int number = stack_pop_number();

    stack_push_number(number);
    stack_push_number(number);
}

static void execute_index() {
    int n = stack_pop_number();
    int n_th_number = stack_peek_n_th_number(n);

    stack_push_number(n_th_number);
}

static void execute_roll() {
    int j = stack_pop_number();
    int n = stack_pop_number();
    j = j % n;

    for (int i = 0; i < j; ++i) {
        stack_roll_once(n);
    }
}

static void execute_if() {
    struct EA_ElementArray* byte_codes = stack_pop_byte_codes();
    int boolean = stack_pop_number();

    assert((1 == boolean) || (0 == boolean));

    if(boolean)
        eval_exec_array(byte_codes);
}

static void execute_exec() {
    struct EA_ElementArray* byte_codes = stack_pop_byte_codes();

    eval_exec_array(byte_codes);
}

static void execute_while() {
    struct EA_ElementArray* body = stack_pop_byte_codes();
    struct EA_ElementArray* cond = stack_pop_byte_codes();

    eval_exec_array(cond);
    int boolean = stack_pop_number();

    while (1 == boolean) {
        eval_exec_array(body);
        eval_exec_array(cond);
        boolean = stack_pop_number();
    }
}

static void register_primitives() {
    dict_put_cfunc("def", execute_def);

    dict_put_cfunc("add", execute_add);
    dict_put_cfunc("sub", execute_sub);
    dict_put_cfunc("div", execute_div);
    dict_put_cfunc("mul", execute_mul);

    dict_put_cfunc("ifelse", execute_ifelse);

    dict_put_cfunc("eq", execute_eq);
    dict_put_cfunc("neq", execute_neq);
    dict_put_cfunc("gt", execute_gt);
    dict_put_cfunc("ge", execute_ge);
    dict_put_cfunc("lt", execute_lt);
    dict_put_cfunc("le", execute_le);

    dict_put_cfunc("pop", execute_pop);
    dict_put_cfunc("exch", execute_exch);
    dict_put_cfunc("dup", execute_dup);
    dict_put_cfunc("index", execute_index);

    dict_put_cfunc("roll", execute_roll);

    dict_put_cfunc("if", execute_if);
    dict_put_cfunc("exec", execute_exec);

    dict_put_cfunc("while", execute_while);
}

/* Unit tests */


static void eval_with_input(char* input) {
    cl_getc_set_src(input);
    eval();
}

static void assert_true(int actual) {
    assert(1 == actual);
}

static void assert_false(int actual) {
    assert(0 == actual);
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);

}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    eval_with_input(input);
    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_lit() {
    char *input = "/helloworld";
    char *expect = "helloworld";

    eval_with_input(input);
    char *actual = stack_pop_lit_name();

    assert(0 == strcmp(expect, actual));
}

static void test_eval_def() {
    char *input = "/abc 123 def /efg 456 def abc efg add";
    int expect = 579;
    
    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_exec_array() {
    char *input = "/abc {1 2} def abc add";
    int expect = 3;

    eval_with_input(input);
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

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_ifelse_true() {
    char* input = "20 1 {30 add} {20 add} ifelse";
    int expect = 50;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_ifelse_false() {
    char* input = "20 0 {30 add} {20 add} ifelse";
    int expect = 40;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_sub() {
    char* input = "20 10 sub";
    int expect = 10;
    
    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_div() {
    char* input = "35 8 div";
    int expect = 4;
    
    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_mul() {
    char* input = "8 32 mul";
    int expect = 256;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_eq() {
    char* input = "299 299 eq";
    int expect = 1;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_neq() {
    char* input = "299 299 neq";
    int expect = 0;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_gt_true() {
    char* input_expect_true = "2 1 gt";

    eval_with_input(input_expect_true);
    int actual = stack_pop_number();

    assert_true(actual);
}

static void test_eval_gt_false() {
    char* input_expect_false = "1 1 gt";

    eval_with_input(input_expect_false);
    int actual = stack_pop_number();

    assert_false(actual);
}

static void test_eval_ge_true() {
    char* input_expect_true = "1 1 ge";

    eval_with_input(input_expect_true);
    int actual = stack_pop_number();

    assert_true(actual);
}

static void test_eval_lt_true() {
    char* input_expect_true = "1 2 lt";

    eval_with_input(input_expect_true);
    int actual = stack_pop_number();

    assert_true(actual);
}

static void test_eval_le_true() {
    char* input_expect_true = "1 1 ge";

    eval_with_input(input_expect_true);
    int actual = stack_pop_number();

    assert_true(actual);
}

static void test_eval_pop_once() {
    char* input = "1 2 3 pop";
    int expect = 2;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_pop_twice() {
    char* input = "1 2 3 pop pop";
    int expect = 1;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_exch() {
    char* input = "1 2 exch";
    int expect1 = 1;
    int expect2 = 2;

    eval_with_input(input);
    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

static void test_eval_dup() {
    char* input = "1 dup";
    int expect = 1;

    eval_with_input(input);
    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();

    assert(expect == actual1);
    assert(expect == actual2);
}

static void test_eval_index() {
    char* input = "1 2 3 4 5 6 7 8 9 10 \
                   4 index";
    int expect = 6;

    eval_with_input(input);
    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_roll() {
    char* input = "1 2 3 4 \
                   3 2 roll";
    int expect1 = 2;
    int expect2 = 4;
    int expect3 = 3;
    int expect4 = 1;

    stack_clear();
    eval_with_input(input);

    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();
    int actual3 = stack_pop_number();
    int actual4 = stack_pop_number();

    assert(expect1 == actual1);
    assert(expect2 == actual2);
    assert(expect3 == actual3);
    assert(expect4 == actual4);
}

static void test_eval_roll_complex() {
    char* input = "0 1 2 3 4 5 6 7 8 9 10 \
                   11 16 roll";

    stack_clear();
    eval_with_input(input);

    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();
    int actual3 = stack_pop_number();
    int actual4 = stack_pop_number();
    int actual5 = stack_pop_number();
    int actual6 = stack_pop_number();
    int actual7 = stack_pop_number();
    int actual8 = stack_pop_number();
    int actual9 = stack_pop_number();
    int actual10 = stack_pop_number();
    int actual11 = stack_pop_number();

    assert(5 == actual1);
    assert(4 == actual2);
    assert(3 == actual3);
    assert(2 == actual4);
    assert(1 == actual5);
    assert(0 == actual6);
    assert(10 == actual7);
    assert(9 == actual8);
    assert(8 == actual9);
    assert(7 == actual10);
    assert(6 == actual11);
}

static void test_eval_if() {
    char* input = "10 1 {100 add} if";
    int expect = 110;

    eval_with_input(input);

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_exec() {
    char* input = "1 2 3 {100 add} exec";
    int expect = 103;

    eval_with_input(input);

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_while() {
    char* input = "\
        100 \
        /cnt 0 def \
        {cnt 3 lt} \
        {10 add /cnt cnt 1 add def} \
        while";
    int expect = 130;

    eval_with_input(input);

    int actual = stack_pop_number();

    assert(expect == actual);
}

static void test_eval_repeat() {
    char* input = "2 {1 2} repeat";
    int expect1 = 1;
    int expect2 = 2;

    eval_with_input(input);

    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();
    int actual3 = stack_pop_number();
    int actual4 = stack_pop_number();

    assert(expect2 == actual1);
    assert(expect1 == actual2);
    assert(expect2 == actual3);
    assert(expect1 == actual4);
}

static void test_suite() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_lit();
    test_eval_def();

    test_eval_exec_array();
    test_eval_exec_array_complex();

    test_eval_ifelse_true();
    test_eval_ifelse_false();

    test_eval_sub();
    test_eval_div();
    test_eval_mul();

    test_eval_eq();
    test_eval_neq();
    test_eval_gt_true();
    test_eval_gt_false();
    test_eval_ge_true();
    test_eval_lt_true();
    test_eval_le_true();

    test_eval_pop_once();
    test_eval_pop_twice();
    test_eval_exch();
    test_eval_dup();
    test_eval_index();

    test_eval_roll();
    test_eval_roll_complex();

    test_eval_if();
    test_eval_exec();

    test_eval_while();
}

int main() {
    register_primitives();

    test_suite();

    return 0;
}
