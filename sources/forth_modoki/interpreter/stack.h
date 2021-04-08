#include "exec_array.h"

#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 24

enum S_ElementDataType {
    S_NUMBER,
    S_EXE_NAME,
    S_LIT_NAME,
    S_BYTE_CODES,
};

union S_ElementData {
    int number;
    char* name;
    struct EA_ElementArray* byte_codes;
};

struct S_Element {
    enum S_ElementDataType dtype;
    union S_ElementData u;
};

void stack_push_number(int data);
void stack_push_exe_name(char* data);
void stack_push_lit_name(char* data);
void stack_push_byte_codes(struct EA_ElementArray* byte_codes);


void stack_pop(struct S_Element* out_elem);
int stack_pop_number();
char* stack_pop_exe_name();
char* stack_pop_lit_name();
struct EA_ElementArray* stack_pop_byte_codes();

void stack_print_all();

#endif
