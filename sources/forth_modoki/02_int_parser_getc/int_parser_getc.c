#include "clesson.h"
#include <assert.h>

/*
cc cl_getc.c int_parser_getc.c
*/

typedef enum TOKEN_TYPES {
    NUMBERS,
    SPACE
} token_types_t;

int parse_one(int *out_val, token_types_t *out_type, int forward_c) {
    int c;
    if (EOF == forward_c)
        c = cl_getc();
    else
        c = forward_c;

    if (' ' == c) {
        *out_type = SPACE;
        *out_val = ' ';

        while (' ' == c) {
            c = cl_getc();
        }
        return c;
    }
    else if ('0' <= c && '9' >= c) {
        *out_type = NUMBERS;

        while ('0' <= c && '9' >= c) {
            *out_val = (c - '0') + (*out_val * 10);
            c = cl_getc();
        }
        return c;
    }
    else {
        return EOF;
    }
}

void test_parse_one_123() {
    int c;
    int val;
    token_types_t type;

    cl_getc_set_src("123");
    c = parse_one(&val, &type, EOF);

    assert(' ' == c);
    assert(123 == val);
    assert(NUMBERS == type);
}

void test_parse_one_123_456() {
    int chars[3] = {0, 0, 0};
    int vals[3] = {0, 0, 0};
    token_types_t types[3] = {0, 0, 0};

    cl_getc_set_src("123 456");

    chars[0] = parse_one(&vals[0], &types[0], EOF);
    chars[1] = parse_one(&vals[1], &types[1], chars[0]);
    chars[2] = parse_one(&vals[2], &types[2], chars[1]);

    assert(123 == vals[0]);
    assert(' ' == vals[1]);
    assert(456 == vals[2]);
    assert(NUMBERS == types[0]);
    assert(SPACE == types[1]);
    assert(NUMBERS == types[2]);
    assert(' ' == chars[0]);
    assert('4' == chars[1]);
    assert(EOF == chars[2]);
}

int main() {
    int answer1 = 0;
    int space = 0;
    int answer2 = 0;

    // write something here.

   int c;
   token_types_t answer1_type = 0;
   token_types_t space_type = 0;
   token_types_t answer2_type = 0;

   test_parse_one_123();
   test_parse_one_123_456();

   cl_getc_set_src("123 456");

   c = parse_one(&answer1, &answer1_type, EOF);
   c = parse_one(&space, &space_type, c);
   c = parse_one(&answer2, &answer2_type, c);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);

    return 0;
}
