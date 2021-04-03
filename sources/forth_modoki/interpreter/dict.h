#include <stdbool.h>

#ifndef DICT_H
#define DICT_H

enum D_ElementDataType {
    D_NUMBER,
    D_C_FUNC,
    D_BYTE_CODES
};

union D_ElementData {
    int number;
    void (*cfunc)();
    struct EA_ElementArray* byte_codes;
};

struct D_Element {
    enum D_ElementDataType dtype;
    union D_ElementData u;
};

bool dict_key_isused(char* key);

void dict_put_number(char* key, int number);
void dict_put_cfunc(char* key, void (*cfunc)());
void dict_put_byte_codes(char* key, struct EA_ElementArray* byte_codes);

struct D_Element* dict_get(char* key);

#endif
