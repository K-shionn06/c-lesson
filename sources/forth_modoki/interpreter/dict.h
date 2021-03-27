#include <stdbool.h>

bool streq(const char* s1, const char* s2);

bool dict_key_isused(char* key);
void dict_put_number(char* key, int data);
int dict_get_number(char* key);
