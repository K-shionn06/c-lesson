#include "clesson.h"
#include <assert.h>

/*
cc cl_getc.c int_parser_getc.c
*/

enum TYPES {
    NUMBERS,
    SPACE
};

char parse_one(int *out_val, int *out_type, char forward_c) {
    char c = forward_c;
    if ('\0' == forward_c)
        c = cl_getc();

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
            *out_val = ((int) c - 48) + (*out_val * 10);
            c = cl_getc();
        }
        return c;
    }
    else {
        return '\0';
    }
}

void test_parse_one_123() {
    char c;
    int val, type;

    cl_getc_set_src("123");
    c = parse_one(&val, &type, '\0');

    assert(' ' == c);
    assert(123 == val);
    assert(NUMBERS == type);
}

int main() {
    int answer1 = 0;
    int space = 0;
    int answer2 = 0;

    // write something here.

   char c;
   int answer1_type = 0;
   int space_type = 0;
   int answer2_type = 0;

   test_parse_one_123();

   cl_getc_set_src("123 456");

   c = parse_one(&answer1, &answer1_type, '\0');
   c = parse_one(&space, &space_type, c);
   c = parse_one(&answer2, &answer2_type, c);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);

    return 0;
}
