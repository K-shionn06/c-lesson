#include <stdbool.h>

#ifndef EXEC_ARRAY_H
#define EXEC_ARRAY_H

enum EA_ElementDataType {
    EA_NUMBER,
    EA_EXE_NAME,
    EA_LIT_NAME,
    EA_V_FUNC,
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

void compile_exec_array(struct EA_Element* out_element);
bool is_same_byte_codes(
    struct EA_ElementArray* elem_ary1,
    struct EA_ElementArray* elem_ary2);

#endif
