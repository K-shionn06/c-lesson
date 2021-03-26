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

void stack_print_all();

void stack_push_number(int);
void stack_push_exe_name(char *);
void stack_push_lit_name(char *);

int stack_pop_number();
char *stack_pop_exe_name();
char *stack_pop_lit_name();
