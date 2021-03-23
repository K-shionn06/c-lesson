#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>


bool streq(const char *s1, const char *s2) {
    if (0 == strcmp(s1, s2))
        return true;
    else
        return false;
}

void execute_add() {
    int a, b;
    bool succ;
    stack_data_t out_stack_data;

    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            a = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }
    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            b = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }

    stack_push_number(a + b);
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

    int actual = 0;
    bool succ;
    stack_data_t out_stack_data;

    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            actual = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }

    assert(expect == actual);

}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    int actual1 = 0;
    int actual2 = 0;
    bool succ;
    stack_data_t out_stack_data;

    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            actual1 = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }
    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            actual2 = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}


static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    int actual = 0;
    bool succ;
    stack_data_t out_stack_data;

    succ = stack_pop(&out_stack_data);
    if (succ) {
        if (S_NUMBER == out_stack_data.dtype) {
            actual = out_stack_data.u.number;
        }
    }
    else {
        assert(0);
    }

    assert(expect == actual);
}


int main() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();

    return 0;
}
