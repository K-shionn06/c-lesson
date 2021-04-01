#ifndef STACK_H
#define STACK_H

enum S_ElementDataType {
    S_NUMBER,
    S_EXE_NAME,
    S_LIT_NAME,
    S_BYTE_CODES,
};

union S_ElementData {
    int number;
    char* name;
};

struct S_Element {
    enum S_ElementDataType dtype;
    union S_ElementData u;
};

void stack_push_number(int);
void stack_push_exe_name(char*);
void stack_push_lit_name(char*);

int stack_pop_number();
char* stack_pop_exe_name();
char* stack_pop_lit_name();

void stack_print_all();

#endif
