#include "exec_array.h"
#include "parser.h"
#include "streq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_NAME_OP_NUMBERS 256


static void copy_element_type(struct EA_Element* elem, enum EA_ElementDataType dtype) {
    elem->dtype = dtype;
}

static void copy_element_number(struct EA_Element* elem, int number) {
    elem->u.number = number;
}

static void copy_element_string(struct EA_Element* elem, char* string) {
    elem->u.name = string;
}

static struct EA_ElementArray* create_empty_elementArray(unsigned int size) {
    struct EA_ElementArray* elem_ary;
    size_t elem_ary_size = 
        sizeof(struct EA_ElementArray) + (sizeof(struct EA_Element) * size);

    elem_ary = (struct EA_ElementArray*)malloc(elem_ary_size);

    assert(NULL != elem_ary);

    elem_ary->len = size;
    return elem_ary;
}

static void copy_array(
    struct EA_Element* dst_array,
    struct EA_Element* src_array,
    size_t size)
{
    memcpy(dst_array, src_array, sizeof(struct EA_Element) * size);
}

int compile_exec_array(int prev_ch, struct EA_Element* out_element) {
    int ch = prev_ch;
    struct Token token = {
        UNKNOWN,
        {0}
    };
    struct EA_Element local_array[MAX_NAME_OP_NUMBERS];
    memset(local_array, 0, sizeof(struct EA_Element) * MAX_NAME_OP_NUMBERS);
    struct EA_ElementArray* elem_ary;

    int i = 0;
    while (i < MAX_NAME_OP_NUMBERS) {
        ch = parse_one(ch, &token);

        switch (token.ltype) {
            default:
                assert(false);

            case SPACE:
                break;

            case NUMBER:
                copy_element_type(&local_array[i], EA_NUMBER);
                copy_element_number(&local_array[i], token.u.number);
                i++;
                break;

            case EXECUTABLE_NAME:
                copy_element_type(&local_array[i], EA_EXE_NAME);
                copy_element_string(&local_array[i], token.u.name);
                i++;
                break;

            case LITERAL_NAME: 
                copy_element_type(&local_array[i], EA_LIT_NAME);
                copy_element_string(&local_array[i], token.u.name);
                i++;
                break;

            case OPEN_CURLY:
                ch = compile_exec_array(ch, &local_array[i]);
                i++;
                break;

            case CLOSE_CURLY:
                elem_ary = create_empty_elementArray(i+1);
                copy_array(elem_ary->elements, local_array, elem_ary->len);
                out_element->dtype = EA_BYTE_CODES;
                out_element->u.byte_codes = elem_ary;
                return ch;
        }
    }
}


/* unit test */


bool is_same_byte_codes(
    struct EA_ElementArray* elem_ary1,
    struct EA_ElementArray* elem_ary2)
{
    if (elem_ary1->len != elem_ary2->len)
        return false;

    int len = elem_ary1->len;
    struct EA_Element *elem1, *elem2;

    for (int i = 0; i < len; i++) {
        elem1 = &elem_ary1->elements[i];
        elem2 = &elem_ary2->elements[i];

        if (elem1->dtype != elem2->dtype)
            return false;

        switch (elem1->dtype) {
            default:
                assert(false);

            case EA_NUMBER:
                if (elem1->u.number != elem2->u.number)
                    return false;
                break;

            case EA_EXE_NAME:
            case EA_LIT_NAME:
                if ( !streq(elem1->u.name, elem2->u.name) )
                    return false;
                break;

            case EA_BYTE_CODES:
                if ( !is_same_byte_codes(elem1->u.byte_codes, elem2->u.byte_codes) )
                    return false;
                break;
        }
    }
    return true;
}

void dump_element_array(struct EA_Element* elem, int size) {
    puts("-----------------");
    puts("idx | type, data");
    puts("-----------------");
    for (int i = 0; i < size-1; i++) {
        switch (elem[i].dtype) {
            default:
                assert(false);

            case EA_NUMBER:
                printf("%03d |  num, %d\n", i, elem[i].u.number);
                break;
            case EA_EXE_NAME:
                printf("%03d |  exe, %s\n", i, elem[i].u.name);
                break;
            case EA_LIT_NAME:
                printf("%03d |  lit, %s\n", i, elem[i].u.name);
                break;
            case EA_BYTE_CODES:
                dump_element_array(
                    elem[i].u.byte_codes->elements,
                    elem[i].u.byte_codes->len);
        }
    }
    puts("-----------------");
}

static void assert_array_element_number(int idx, int expect, struct EA_Element* array) {
    assert(EA_NUMBER == array[idx].dtype);
    assert(expect == array[idx].u.number);
}

static void assert_array_element_string(
    int idx,
    char* expect,
    struct EA_Element* array)
{
    assert((EA_EXE_NAME == array[idx].dtype) || (EA_LIT_NAME == array[idx].dtype));
    assert(0 == strcmp(expect, array[idx].u.name));
}

static void test_compile_exec_array_number() {
    char* input = "1 2 3 4 5 6 7 8 9 10 }";
    cl_getc_set_src(input);

    struct EA_Element elem;
    compile_exec_array(EOF, &elem);
    dump_element_array(elem.u.byte_codes->elements, elem.u.byte_codes->len); // debug

    assert(EA_BYTE_CODES == elem.dtype);

    assert_array_element_number(0, 1, elem.u.byte_codes->elements);
    assert_array_element_number(1, 2, elem.u.byte_codes->elements);
    assert_array_element_number(2, 3, elem.u.byte_codes->elements);
    assert_array_element_number(3, 4, elem.u.byte_codes->elements);
    assert_array_element_number(4, 5, elem.u.byte_codes->elements);
    assert_array_element_number(5, 6, elem.u.byte_codes->elements);
    assert_array_element_number(6, 7, elem.u.byte_codes->elements);
    assert_array_element_number(7, 8, elem.u.byte_codes->elements);
    assert_array_element_number(8, 9, elem.u.byte_codes->elements);
    assert_array_element_number(9, 10, elem.u.byte_codes->elements);
}

static void test_compile_exec_array_mix() {
    char* input = "3 add 3 2999 sam tom bob 499 2000}";
    cl_getc_set_src(input);

    struct EA_Element elem;
    compile_exec_array(EOF, &elem);
    dump_element_array(elem.u.byte_codes->elements, elem.u.byte_codes->len); // debug

    assert(EA_BYTE_CODES == elem.dtype);

    assert_array_element_number(0, 3, elem.u.byte_codes->elements);
    assert_array_element_string(1, "add", elem.u.byte_codes->elements);
    assert_array_element_number(2, 3, elem.u.byte_codes->elements);
    assert_array_element_number(3, 2999, elem.u.byte_codes->elements);
    assert_array_element_string(4, "sam", elem.u.byte_codes->elements);
    assert_array_element_string(5, "tom", elem.u.byte_codes->elements);
    assert_array_element_string(6, "bob", elem.u.byte_codes->elements);
    assert_array_element_number(7, 499, elem.u.byte_codes->elements);
    assert_array_element_number(8, 2000, elem.u.byte_codes->elements);
}

static void test_compile_exec_array_nest() {
    char* input = "12 34 /efg { 123 add } def }";
    cl_getc_set_src(input);

    struct EA_Element elem;
    compile_exec_array(EOF, &elem);
    dump_element_array(elem.u.byte_codes->elements, elem.u.byte_codes->len); // debug

    assert_array_element_number(0, 12, elem.u.byte_codes->elements);
    assert_array_element_number(1, 34, elem.u.byte_codes->elements);
    assert_array_element_string(2, "efg", elem.u.byte_codes->elements);

    assert_array_element_number(
        0,
        123,
        elem.u.byte_codes->elements[3].u.byte_codes->elements);
    assert_array_element_string(
        1,
        "add",
        elem.u.byte_codes->elements[3].u.byte_codes->elements);

    assert_array_element_string(4, "def", elem.u.byte_codes->elements);
}

#if 0
int main() {
    test_compile_exec_array_number();
    test_compile_exec_array_mix();
    test_compile_exec_array_nest();

    return 0;
}
#endif
