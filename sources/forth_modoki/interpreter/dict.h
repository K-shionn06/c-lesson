#include <stdbool.h>

enum D_ElemValueType {
    V_NUMBER,
    V_C_FUNC
};

struct D_ElemValue {
    enum D_ElemValueType vtype;
    union {
        int number;
        void (*cfunc)();
    } u;
};

bool streq(const char* s1, const char* s2);

bool dict_key_isused(char* key);

void dict_put_number(char* key, int number);
void dict_put_cfunc(char* key, void (*cfunc)());

struct D_ElemValue* dict_get(char* key);
