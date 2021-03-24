#include <stdbool.h>


enum StackDataType {
    S_NUMBER,
    S_EXE_NAME,
    S_LIT_NAME
};

union StackData {
    int number;
    char *name;
};

typedef struct {
    enum StackDataType dtype;
    union StackData u;
} stack_data_t;

void stack_clear();
void stack_print_all();

bool stack_isfull();
bool stack_isempty();

void stack_push_number(int);
void stack_push_exe_name(char *);
void stack_push_lit_name(char *);
void stack_pop(stack_data_t *);

void stack_pop_number(int *);
void stack_pop_exe_name(char **);
void stack_pop_lit_name(char **);
