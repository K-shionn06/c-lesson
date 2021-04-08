#include "stack.h"
#include "clesson.h"
#include "streq.h"
#include "exec_array.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


struct Stack {
    unsigned int pos;
    struct S_Element array[STACK_SIZE];
};


struct Stack global_stack;


/* core functions for global_stack */


void stack_clear() {
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

void stack_push_byte_codes(struct EA_ElementArray* byte_codes) {
    assert( !stack_isfull() );

    global_stack.array[global_stack.pos].dtype = S_BYTE_CODES;
    global_stack.array[global_stack.pos].u.byte_codes = byte_codes;
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

void stack_pop(struct S_Element* out_elem) {
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
        case S_BYTE_CODES:
            out_elem->u.byte_codes = global_stack.array[global_stack.pos].u.byte_codes;
            break;
    }
}

int stack_pop_number() {
    struct S_Element elem;
    stack_pop(&elem);

    assert(S_NUMBER == elem.dtype);

    return elem.u.number;
}

struct EA_ElementArray* stack_pop_byte_codes() {
    struct S_Element elem;
    stack_pop(&elem);

    assert(S_BYTE_CODES == elem.dtype);

    return elem.u.byte_codes;
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

static void stack_peek_n_th(int n, struct S_Element* out_elem) {
    int n_th_pos = global_stack.pos - (n+1);

    out_elem->dtype = global_stack.array[n_th_pos].dtype;

    switch (out_elem->dtype) {
        case S_NUMBER:
            out_elem->u.number = global_stack.array[n_th_pos].u.number;
            break;
        case S_EXE_NAME:
        case S_LIT_NAME:
            out_elem->u.name = global_stack.array[n_th_pos].u.name;
            break;
        case S_BYTE_CODES:
            out_elem->u.byte_codes = global_stack.array[n_th_pos].u.byte_codes;
            break;
    }
}

int stack_peek_n_th_number(int n) {
    struct S_Element elem;
    stack_peek_n_th(n, &elem);

    assert(S_NUMBER == elem.dtype);

    return elem.u.number;
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
                printf("     | %p | -> %s\n", 
                    global_stack.array[i].u.name,
                    global_stack.array[i].u.name);
                break;

            case S_BYTE_CODES:
                printf("     | %p | -> byte_codes\n", 
                    global_stack.array[i].u.byte_codes);
                break;
        }

        if (global_stack.pos == i) 
            printf("%04d |----------------| POS\n", i);
        else
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

/* unit test for EA_ElementArray a.k.a byte_codes */

static void assert_stack_array_byte_codes(
    int idx, 
    struct EA_ElementArray* expect_byte_codes)
{
    assert(S_BYTE_CODES == global_stack.array[idx].dtype);
    struct EA_ElementArray* actual_byte_codes = global_stack.array[idx].u.byte_codes;
    assert( is_same_byte_codes(expect_byte_codes, actual_byte_codes) );
}

static void call_compile_exec_array(
    char* input,
    struct EA_Element* out_elem)
{
    cl_getc_set_src(input);
    compile_exec_array(EOF, out_elem);
} 

static void test_push_byte_codes() {
    struct EA_Element input_elem;
    call_compile_exec_array("1 2 3 4 5 6 add sum}", &input_elem);
    stack_clear();

    stack_push_byte_codes(input_elem.u.byte_codes);

    assert_stack_array_byte_codes(0, input_elem.u.byte_codes);
}

static void test_push_two_byte_codes() {
    struct EA_Element input_elem1;
    call_compile_exec_array("1 2 3 4 5 6 add sum}", &input_elem1);
    struct EA_Element input_elem2;
    call_compile_exec_array("6 5 4 3 2 1 add sum}", &input_elem2);
    stack_clear();

    stack_push_byte_codes(input_elem1.u.byte_codes);
    stack_push_byte_codes(input_elem2.u.byte_codes);

    assert_stack_array_byte_codes(0, input_elem1.u.byte_codes);
    assert_stack_array_byte_codes(1, input_elem2.u.byte_codes);
}

static void test_pop_byte_codes() {
    struct EA_Element input_elem;
    call_compile_exec_array("1 2 3 4 5 6 add sum}", &input_elem);
    stack_clear();
    stack_push_byte_codes(input_elem.u.byte_codes);

    struct EA_ElementArray* actual_byte_codes = stack_pop_byte_codes();

    assert( is_same_byte_codes(input_elem.u.byte_codes, actual_byte_codes) );
}

static void test_pop_two_byte_codes() {
    struct EA_Element input_elem1;
    call_compile_exec_array("1 2 3 4 5 6 add sum}", &input_elem1);
    struct EA_Element input_elem2;
    call_compile_exec_array("6 5 4 3 2 1 add sum}", &input_elem2);
    stack_clear();
    stack_push_byte_codes(input_elem1.u.byte_codes);
    stack_push_byte_codes(input_elem2.u.byte_codes);

    struct EA_ElementArray* actual_byte_codes2 = stack_pop_byte_codes();
    struct EA_ElementArray* actual_byte_codes1 = stack_pop_byte_codes();

    assert( is_same_byte_codes(input_elem1.u.byte_codes, actual_byte_codes1) );
    assert( is_same_byte_codes(input_elem2.u.byte_codes, actual_byte_codes2) );
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

    test_push_byte_codes();
    test_push_two_byte_codes();
    test_pop_byte_codes();
    test_pop_two_byte_codes();
}

#if 0
int main() {
    test_suite();
    return 0;
}
#endif
