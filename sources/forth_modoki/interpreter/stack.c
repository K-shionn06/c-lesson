#include "stack.h"
#include "streq.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define STACK_SIZE 1024

struct Stack {
    unsigned int pos;
    struct S_Element array[STACK_SIZE];
};


struct Stack global_stack;


/* core functions for global_stack */


static void stack_clear() {
    for (int i = 0; i < STACK_SIZE; i++) {
        global_stack.array[i].dtype = S_NUMBER;
        global_stack.array[i].u.number = 0;
    }
    global_stack.pos = 0;
}

static bool stack_isfull() {
    if (STACK_SIZE == global_stack.pos)
        return true;
    else
        return false;
}

static bool stack_isempty() {
    if (0 == global_stack.pos)
        return true;
    else
        return false;
}

void stack_push_number(int data) {
    assert( !stack_isfull() );

    global_stack.array[global_stack.pos].dtype = S_NUMBER;
    global_stack.array[global_stack.pos].u.number = data;
    global_stack.pos++;
}

static void stack_push_string(enum S_ElementDataType dtype, char* data) {
    assert( !stack_isfull() );
    assert((S_EXE_NAME == dtype) || (S_LIT_NAME == dtype));

    global_stack.array[global_stack.pos].dtype = dtype;
    global_stack.array[global_stack.pos].u.name = data;
    global_stack.pos++;
}

void stack_push_exe_name(char* data) {
    stack_push_string(S_EXE_NAME, data);
}
void stack_push_lit_name(char* data) {
    stack_push_string(S_LIT_NAME, data);
}

static void stack_pop(struct S_Element* out_elem) {
    assert( !stack_isempty() );

    global_stack.pos--;
    out_elem->dtype = global_stack.array[global_stack.pos].dtype;

    switch (out_elem->dtype) {
        case S_NUMBER:
            out_elem->u.number = global_stack.array[global_stack.pos].u.number;
            break;
        case S_EXE_NAME:
        case S_LIT_NAME:
            out_elem->u.name = global_stack.array[global_stack.pos].u.name;
            break;
    }
}

int stack_pop_number() {
    struct S_Element elem;
    stack_pop(&elem);

    assert(S_NUMBER == elem.dtype);

    return elem.u.number;
}

static char* stack_pop_string(enum S_ElementDataType dtype) {
    assert((S_EXE_NAME == dtype) || (S_LIT_NAME == dtype));

    struct S_Element elem;
    stack_pop(&elem);

    assert(dtype == elem.dtype);

    return elem.u.name;
}

char* stack_pop_exe_name() {
    return stack_pop_string(S_EXE_NAME);
}

char* stack_pop_lit_name() {
    return stack_pop_string(S_LIT_NAME);
}

void stack_print_all() {
    int i;
    puts("-----------------------------");
    for (i = STACK_SIZE-1; i >= 0; i--) {
        switch (global_stack.array[i].dtype) {
            case S_NUMBER:
                printf("     | %014d |\n", global_stack.array[i].u.number);
                break;
            case S_EXE_NAME:
            case S_LIT_NAME:
                printf("     | %p | -> %s\n", global_stack.array[i].u.name,
                                                global_stack.array[i].u.name);
                break;
        }
        printf("%04d |----------------|\n", i);
    }
    puts("-----------------------------");
}


/* unit tests */


static void test_stack_push_number_123() {
    int input = 123;

    stack_clear();
    stack_push_number(input);

    assert(S_NUMBER == global_stack.array[0].dtype);
    assert(input == global_stack.array[0].u.number);
}

static void test_stack_push_string_as_exe() {
    char* input = "helloworld";

    stack_clear();
    stack_push_string(S_EXE_NAME, input);

    assert(S_EXE_NAME == global_stack.array[0].dtype);
    assert(streq(input, global_stack.array[0].u.name));
}

static void test_stack_push_string_as_lit() {
    char* input = "helloworld";

    stack_clear();
    stack_push_string(S_LIT_NAME, input);

    assert(S_LIT_NAME == global_stack.array[0].dtype);
    assert(streq(input, global_stack.array[0].u.name));
}

static void test_stack_pop_when_empty() {
    // This should fail

    struct S_Element elem;

    stack_clear();
    stack_pop(&elem);
}

static void test_stack_pop_123() {
    int input = 123, actual;

    stack_clear();
    stack_push_number(input);
    actual = stack_pop_number();

    assert(input == actual);
}

static void test_stack_pop_exe_name() {
    char* input = "hellowrold";
    char* actual;

    stack_clear();
    stack_push_string(S_EXE_NAME, input);

    actual = stack_pop_string(S_EXE_NAME);

    assert(streq(input, actual));
}

static void test_stack_pop_lit_name() {
    char* input = "hellowrold";
    char* actual;

    stack_clear();
    stack_push_string(S_LIT_NAME, input);

    actual = stack_pop_string(S_LIT_NAME);

    assert(streq(input, actual));
}

static void test_stack_isfull() {
    int input = 123;
    bool actual;

    stack_clear();
    for (int i = 0; i < STACK_SIZE; i++) {
        stack_push_number(input);
    }
    actual = stack_isfull();

    assert(true == actual);
}

static void test_stack_isempty() {
    bool actual;

    stack_clear();
    actual = stack_isempty();

    assert(true == actual);
}

static void test_suite() {
    test_stack_push_number_123();
    // test_stack_pop_when_empty();

    test_stack_push_string_as_exe();
    test_stack_push_string_as_lit();

    test_stack_pop_123();

    test_stack_pop_exe_name();
    test_stack_pop_lit_name();

    test_stack_isempty();
    test_stack_isfull();
}

#if 0
int main() {
    test_suite();
    return 0;
}
#endif
