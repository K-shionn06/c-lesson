#include <stdbool.h>
#include "stack.h"

typedef stack_data_t dict_value_t;

bool streq(const char *s1, const char *s2);

int dict_get_idx(char *key);
void dict_put_number(char *key, int data);
int dict_get_number(char *key);
void dict_print_all();
