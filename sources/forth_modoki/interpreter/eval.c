#include "clesson.h"
#include "parser.h"
#include "dict.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>


static void execute_add() {
    int number1 = stack_pop_number();
    int number2 = stack_pop_number();

    stack_push_number(number1 + number2);
}

static void execute_def() {
    int number = stack_pop_number();
    char *lit_name = stack_pop_lit_name();
    
    dict_put_number(lit_name, number);
}

void eval() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };

    do {
        ch = parse_one(ch, &token);
        if (UNKNOWN!= token.ltype) {
            switch (token.ltype) {
                case NUMBER:
                    stack_push_number(token.u.number);
                    break;

                case EXECUTABLE_NAME:
                    if (streq("add", token.u.name)) {
                        execute_add();
                    }
                    else if (streq("def", token.u.name)) {
                        execute_def();
                    }
                    else if (dict_key_isused(token.u.name)) {
                        int number = dict_get_number(token.u.name);
                        stack_push_number(number);
                    }
                    else {
                        assert(false);
                    }
                    break;

                case LITERAL_NAME:
                    stack_push_lit_name(token.u.name);
                    break;
                default:
                    break;
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
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_lit();
    test_eval_def();

    return 0;
}
