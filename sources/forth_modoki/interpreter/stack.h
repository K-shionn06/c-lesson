void stack_push_number(int);
void stack_push_exe_name(char*);
void stack_push_lit_name(char*);

int stack_pop_number();
char* stack_pop_exe_name();
char* stack_pop_lit_name();

void stack_print_all();
