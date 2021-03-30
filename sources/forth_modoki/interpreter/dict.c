#include "dict.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TABLE_SIZE 1024

struct D_Elem {
    char* key;
    struct D_ElemValue value;
    struct D_Elem* next;
};


struct D_Elem* array[TABLE_SIZE];


/* functions */


bool streq(const char* s1, const char* s2) {
    if (0 == strcmp(s1, s2))
        return true;
    else
        return false;
}

static int hash(char* str) {
    unsigned int val = 0;
    while (*str) {
        val += *str++;
    }
    return (int)(val % 1024);
}

static void copy_elem_value(struct D_ElemValue* dest_value, struct D_ElemValue* src_value) {
    dest_value->vtype = src_value->vtype;

    switch (src_value->vtype) {
        default:
            assert(false);

        case V_NUMBER:
            dest_value->u.number = src_value->u.number;
            break;
        case V_C_FUNC:
            dest_value->u.cfunc = src_value->u.cfunc;
            break;
    }
}

static struct D_Elem* new_elem(char* key, struct D_ElemValue* value) {
    struct D_Elem* elem;
    elem = (struct D_Elem*)malloc(sizeof(struct D_Elem));

    elem->key = key;
    copy_elem_value(&elem->value, value);
    elem->next = NULL;

    return elem;
}

static struct D_Elem* search_elem_by_key(struct D_Elem* head_elem, char* key) {
    struct D_Elem* elem;
    elem = head_elem;

    while (NULL != elem) {
        if (streq(key, elem->key))
            return elem;
        else
            elem = elem->next;
    }

    return NULL;
}

static void insert_elem(int idx, char* key, struct D_ElemValue* value) {
    struct D_Elem* head_elem;
    head_elem = array[idx];

    assert(NULL != head_elem);

    array[idx] = new_elem(key, value);
    array[idx]->next = head_elem;
}

static void update_elem(int idx, char* key, struct D_ElemValue* value) {
    struct D_Elem* elem;
    elem = search_elem_by_key(array[idx], key);

    assert(NULL != elem);

    copy_elem_value(&elem->value, value);
}

static void update_or_insert_elem(int idx, char* key, struct D_ElemValue* value) {
    struct D_Elem* elem;
    elem = search_elem_by_key(array[idx], key);

    if (NULL == elem)
        insert_elem(idx, key, value);
    else
        update_elem(idx, key, value);
}

static void dict_put(char* key, struct D_ElemValue* value) {
    int idx = hash(key);

    if (NULL == array[idx]) {
        array[idx] = new_elem(key, value);
    }
    else {
        update_or_insert_elem(idx, key, value);
    }
}

void dict_put_number(char* key, int number) {
    struct D_ElemValue value;
    value.vtype = V_NUMBER;
    value.u.number = number;

    dict_put(key, &value);
}

void dict_put_cfunc(char* key, void (*cfunc)()) {
    struct D_ElemValue value;
    value.vtype = V_C_FUNC;
    value.u.cfunc = cfunc;

    dict_put(key, &value);
}

struct D_ElemValue* dict_get(char* key) {
    int idx = hash(key);

    struct D_Elem* head_elem;
    struct D_Elem* elem;
    head_elem = array[idx];
    elem = search_elem_by_key(head_elem, key);

    return &elem->value;
}

bool dict_key_isused(char* key) {
    int idx = hash(key);
    struct D_Elem* elem = search_elem_by_key(array[idx], key);

    if (NULL != elem)
        return true;
    else
        return false;
}


/* unit test */


static void clear_array_for_test() {
    // This function only for test

    for (int i = 0; i < TABLE_SIZE; i++) {
        array[i] = NULL;
    }
}

static void test_dict_put_number() {
    char* input_key = "yellow";
    int idx = hash(input_key);
    int input_number = 2000;

    clear_array_for_test();
    dict_put_number(input_key, input_number);

    assert(V_NUMBER == array[idx]->value.vtype);
    assert(input_number == array[idx]->value.u.number);
}

static void test_dict_put_number_same_hash_two_numbers() {
    char* input_key1 = "abc";
    char* input_key2 = "cba";
    int idx = hash(input_key1);
    int input_number1 = 100;
    int input_number2 = 200;

    assert(hash(input_key1) == hash(input_key2));

    clear_array_for_test();
    dict_put_number(input_key1, input_number1);
    dict_put_number(input_key2, input_number2);

    assert(V_NUMBER == array[idx]->next->value.vtype);
    assert(V_NUMBER == array[idx]->value.vtype);
    assert(input_number1 == array[idx]->next->value.u.number);
    assert(input_number2 == array[idx]->value.u.number);
}

static void sample_cfunc() {
    puts("hello cfunc");
}

static void test_dict_put_cfunc() {
    char* input_key = "pen";
    int idx = hash(input_key);
    void (* input_func)() = sample_cfunc;

    clear_array_for_test();
    dict_put_cfunc(input_key, input_func);

    assert(V_C_FUNC == array[idx]->value.vtype);
    assert(input_func == array[idx]->value.u.cfunc);
}

static void test_dict_get_type_number() {
    char* input_key = "yellow";
    int input_number = 2000;

    clear_array_for_test();
    dict_put_number(input_key, input_number);
    struct D_ElemValue* value = dict_get(input_key);

    assert(V_NUMBER == value->vtype);
    assert(input_number == value->u.number);
}

static void test_dict_get_type_number_same_hash_two_numbers() {
    char* input_key1 = "abc";
    char* input_key2 = "cba";
    int input_number1 = 100;
    int input_number2 = 200;

    assert(hash(input_key1) == hash(input_key2));

    clear_array_for_test();
    dict_put_number(input_key1, input_number1);
    dict_put_number(input_key2, input_number2);

    struct D_ElemValue* value1 = dict_get(input_key1);
    struct D_ElemValue* value2 = dict_get(input_key2);

    assert(V_NUMBER == value1->vtype);
    assert(V_NUMBER == value2->vtype);
    assert(input_number1 == value1->u.number);
    assert(input_number2 == value2->u.number);
}

static void test_dict_get_type_cfunc() {
    char* input_key = "pen";
    void (* input_func)() = sample_cfunc;

    clear_array_for_test();
    dict_put_cfunc(input_key, input_func);
    struct D_ElemValue* value = dict_get(input_key);

    assert(V_C_FUNC == value->vtype);
    assert(input_func == value->u.cfunc);
}


#if 0
int main() {
    test_dict_put_number();
    test_dict_put_number_same_hash_two_numbers();
    test_dict_put_cfunc();
    test_dict_get_type_number();
    test_dict_get_type_number_same_hash_two_numbers();
    test_dict_get_type_cfunc();

    return 0;
}
#endif
