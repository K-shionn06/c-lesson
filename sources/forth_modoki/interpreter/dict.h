#include "stack.h"


typedef stack_data_t dict_value_t;

void dict_put_number(char *key, int data);
void dict_put_lit_name(char *key, char *data);
int dict_get_number(char *key);
