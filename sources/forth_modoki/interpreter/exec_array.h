#include <stdbool.h>

#ifndef EXEC_ARRAY_H
#define EXEC_ARRAY_H

enum EA_ElementDataType {
    EA_NUMBER,
    EA_EXE_NAME,
    EA_LIT_NAME,
    EA_BYTE_CODES,
};

union EA_ElementData {
    int number;
    char* name;
    void (*cfunc)();
    struct EA_ElementArray* byte_codes;
};

struct EA_Element {
    enum EA_ElementDataType dtype;
    union EA_ElementData u;
};

struct EA_ElementArray {
    int len;
    struct EA_Element elements[];
};

/* Returns look-ahead character */
int compile_exec_array(int prev_ch, struct EA_Element* out_element);

/* Check given byte_codes are same. */
bool is_same_byte_codes(
    struct EA_ElementArray* elem_ary1,
    struct EA_ElementArray* elem_ary2);

/* elem is the first element of byte_codes->elements array */
void dump_element_array(struct EA_Element* elem, int size);

void call_compile_exec_array(char* input, struct EA_Element* out_elem);

#endif
