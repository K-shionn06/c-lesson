#include "clesson.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define NAME_SIZE 256

static bool _isdigit(int c) {
    if (('0' <= c) && ('9' >= c))
        return true;
    
    return false;
}

static bool _isalpha(int c) {
    if (('A' <= c) && ('Z' >= c))
        return true;
    if (('a' <= c) && ('z' >= c))
        return true;
    
    return false;
}

static bool _isname(int c) {
    if (_isdigit(c) || _isalpha(c))
        return true;
    if (('-' == c) || ('_' == c))
        return true;

    return false;
}

int parse_one(int prev_ch, struct Token* out_token) {
    int c;

    if (EOF == prev_ch)
        c = cl_getc(); 
    else
        c = prev_ch;

    if (' ' == c) {
        // ltype: SPACE

        out_token->ltype = SPACE;
        out_token->u.onechar = ' ';

        while (' ' == c) {
            c = cl_getc();
        }

        return c;
    }
    else if ('{' == c) {
        // ltype: OPEN_CURLY

        out_token->ltype = OPEN_CURLY;
        out_token->u.onechar = '{';

        while ('{' == c) {
            c = cl_getc();
        }

        return c;
    }
    else if ('}' == c) {
        // ltype: CLOSE+CURLY

        out_token->ltype = CLOSE_CURLY;
        out_token->u.onechar = '}';

        while ('}' == c) {
            c = cl_getc();
        }

        return c;
    }
    else if (_isdigit(c)) {
        // ltype: NUMBER

        out_token->ltype = NUMBER;
        out_token->u.number = 0;
        
        while (_isdigit(c)) {
            out_token->u.number = (c - '0') + (out_token->u.number * 10);
            c = cl_getc();
        }

        return c;
    }
    else if (_isalpha(c)) {
        // ltype: EXECUTABLE_NAME

        char name_p[NAME_SIZE];
        memset(name_p, 0, NAME_SIZE);

        for (int i = 0; i < (NAME_SIZE - 1); i++) {
            if (_isname(c)) {
                name_p[i] = c;
                c = cl_getc();
            }
            else {
                out_token->ltype = EXECUTABLE_NAME;
                out_token->u.name = (char*)malloc(i + 1);
                strcpy(out_token->u.name, name_p);

                return c;
            }
        }
    }
    else if ('/' == c) {
        // ltype: LITERAL_NAME

        char name_p[NAME_SIZE]; 
        memset(name_p, 0, NAME_SIZE);

        c = cl_getc();
        for (int i = 0; i < (NAME_SIZE - 1); i++) {
            if (_isname(c)) {
                name_p[i] = c;
                c = cl_getc();
            }
            else {
                out_token->ltype = LITERAL_NAME;
                out_token->u.name = (char*)malloc(i + 1);
                strcpy(out_token->u.name, name_p);

                return c;
            }
        }
    }
    else if (EOF == c) {
        // ltype: END_OF_FILE

        out_token->ltype = END_OF_FILE;

        return EOF;
    }

    out_token->ltype = UNKNOWN;
    return EOF;
}

static void parser_print_all() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };

    do {
        ch = parse_one(ch, &token);
        if(token.ltype != UNKNOWN) {
            switch(token.ltype) {
                case NUMBER:
                    printf("num: %d\n", token.u.number);
                    break;
                case SPACE:
                    printf("space!\n");
                    break;
                case OPEN_CURLY:
                    printf("Open curly brace '%c'\n", token.u.onechar);
                    break;
                case CLOSE_CURLY:
                    printf("Close curly brace '%c'\n", token.u.onechar);
                    break;
                case EXECUTABLE_NAME:
                    printf("EXECUTABLE_NAME: %s\n", token.u.name);
                    break;
                case LITERAL_NAME:
                    printf("LITERAL_NAME: %s\n", token.u.name);
                    break;

                default:
                    printf("Unknown type %d\n", token.ltype);
                    break;
            }
        }
    }while(ch != EOF);
}


/* unit tests */


static void test_parse_one_number() {
    char* input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char* input = "";
    enum LexicalType expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name() {
    char* input = "add123";
    char* expect_name = "add123";
    enum LexicalType expect_type = EXECUTABLE_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(0 == strcmp(token.u.name, expect_name));
}

static void test_parse_one_literal_name() {
    char* input = "/add123";
    char* expect_name = "add123";
    enum LexicalType expect_type = LITERAL_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(0 == strcmp(token.u.name, expect_name));
}

static void test_parse_one_open_curly() {
    char* input = "{";
    char expect_onechar = '{';
    enum LexicalType expect_type = OPEN_CURLY;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(token.u.onechar == expect_onechar);
}

static void test_parse_one_close_curly() {
    char* input = "}";
    char expect_onechar = '}';
    enum LexicalType expect_type = CLOSE_CURLY;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(token.u.onechar == expect_onechar);
}

static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
    test_parse_one_open_curly();
    test_parse_one_close_curly();
}

#if 0
int main() {
    unit_tests();

    cl_getc_set_src("123 45 add /some { 2 3 add } def");
    parser_print_all();
    return 0;
}
#endif
