#include "dict.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TABLE_SIZE 1024

struct D_ArrayElement {
    char* key;
    struct D_Element elem;
    struct D_ArrayElement* next;
};


struct D_ArrayElement* array[TABLE_SIZE];


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
    return (int)(val % TABLE_SIZE);
}

static void copy_element(struct D_Element* dest, struct D_Element* src) {
    dest->dtype = src->dtype;

    switch (src->dtype) {
        default:
            assert(false);

        case D_NUMBER:
            dest->u.number = src->u.number;
            break;
        case D_C_FUNC:
            dest->u.cfunc = src->u.cfunc;
            break;
    }
}

static struct D_ArrayElement* new_ary_elem(char* key, struct D_Element* elem) {
    struct D_ArrayElement* new_ary_elem;
    new_ary_elem = (struct D_ArrayElement*)malloc(sizeof(struct D_ArrayElement));

    new_ary_elem->key = key;
    copy_element(&new_ary_elem->elem, elem);
    new_ary_elem->next = NULL;

    return new_ary_elem;
}

static struct D_ArrayElement* search_ary_elem_by_key(
    struct D_ArrayElement* head_ary_elem,
    char* key)
{
    struct D_ArrayElement* ary_elem = head_ary_elem;

    while (NULL != ary_elem) {
        if (streq(key, ary_elem->key))
            return ary_elem;
        else
            ary_elem = ary_elem->next;
    }

    return NULL;
}

static void insert_ary_elem(int idx, char* key, struct D_Element* elem) {
    struct D_ArrayElement* head_ary_elem = array[idx];

    assert(NULL != head_ary_elem);

    array[idx] = new_ary_elem(key, elem);
    array[idx]->next = head_ary_elem;
}

static void update_ary_elem(
    struct D_ArrayElement* found_ary_elem,
    struct D_Element* elem)
{
    assert(NULL != found_ary_elem);

    copy_element(&found_ary_elem->elem, elem);
}

static void update_or_insert_ary_elem(int idx, char* key, struct D_Element* elem) {
    struct D_ArrayElement* ary_elem = search_ary_elem_by_key(array[idx], key);

    if (NULL == ary_elem)
        insert_ary_elem(idx, key, elem);
    else
        update_ary_elem(ary_elem, elem);
}

static void dict_put(char* key, struct D_Element* elem) {
    int idx = hash(key);

    if (NULL == array[idx]) {
        array[idx] = new_ary_elem(key, elem);
    }
    else {
        update_or_insert_ary_elem(idx, key, elem);
    }
}

void dict_put_number(char* key, int number) {
    struct D_Element elem = {
        .dtype = D_NUMBER,
        .u.number = number
    };

    dict_put(key, &elem);
}

void dict_put_cfunc(char* key, void (*cfunc)()) {
    struct D_Element elem = {
        .dtype = D_C_FUNC,
        .u.cfunc = cfunc
    };

    dict_put(key, &elem);
}

struct D_Element* dict_get(char* key) {
    int idx = hash(key);

    struct D_ArrayElement* ary_elem = search_ary_elem_by_key(array[idx], key);

    return &ary_elem->elem;
}

bool dict_key_isused(char* key) {
    int idx = hash(key);
    struct D_ArrayElement* ary_elem = search_ary_elem_by_key(array[idx], key);

    if (NULL != ary_elem)
        return true;
    else
        return false;
}


/* unit test */


static void clear_array() {
    struct D_ArrayElement *ary_elem, *nxt_ary_elem;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (NULL == array[i])
             continue;

        ary_elem = array[i];
        while (NULL != ary_elem) {
            nxt_ary_elem = ary_elem->next;
            free(ary_elem);
            ary_elem = nxt_ary_elem;
        }
        array[i] = NULL;
    }
}

static void test_dict_put_number() {
    char* input_key = "yellow";
    int idx = hash(input_key);
    int input_number = 2000;

    clear_array();
    dict_put_number(input_key, input_number);

    assert(D_NUMBER == array[idx]->elem.dtype);
    assert(input_number == array[idx]->elem.u.number);
}

static void test_dict_put_number_same_hash_two_numbers() {
    char* input_key1 = "abc";
    char* input_key2 = "cba";
    int idx = hash(input_key1);
    int input_number1 = 100;
    int input_number2 = 200;

    assert(hash(input_key1) == hash(input_key2));

    clear_array();
    dict_put_number(input_key1, input_number1);
    dict_put_number(input_key2, input_number2);

    assert(D_NUMBER == array[idx]->next->elem.dtype);
    assert(D_NUMBER == array[idx]->elem.dtype);
    assert(input_number1 == array[idx]->next->elem.u.number);
    assert(input_number2 == array[idx]->elem.u.number);
}

static void sample_cfunc() {
    puts("hello cfunc");
}

static void test_dict_put_cfunc() {
    char* input_key = "pen";
    int idx = hash(input_key);
    void (* input_func)() = sample_cfunc;

    clear_array();
    dict_put_cfunc(input_key, input_func);

    assert(D_C_FUNC == array[idx]->elem.dtype);
    assert(input_func == array[idx]->elem.u.cfunc);
}

static void test_dict_get_type_number() {
    char* input_key = "yellow";
    int input_number = 2000;

    clear_array();
    dict_put_number(input_key, input_number);
    struct D_Element* elem = dict_get(input_key);

    assert(D_NUMBER == elem->dtype);
    assert(input_number == elem->u.number);
}

static void test_dict_get_type_number_same_hash_two_numbers() {
    char* input_key1 = "abc";
    char* input_key2 = "cba";
    int input_number1 = 100;
    int input_number2 = 200;

    assert(hash(input_key1) == hash(input_key2));

    clear_array();
    dict_put_number(input_key1, input_number1);
    dict_put_number(input_key2, input_number2);

    struct D_Element* elem1 = dict_get(input_key1);
    struct D_Element* elem2 = dict_get(input_key2);

    assert(D_NUMBER == elem1->dtype);
    assert(D_NUMBER == elem2->dtype);
    assert(input_number1 == elem1->u.number);
    assert(input_number2 == elem2->u.number);
}

static void test_dict_get_type_cfunc() {
    char* input_key = "pen";
    void (* input_func)() = sample_cfunc;

    clear_array();
    dict_put_cfunc(input_key, input_func);
    struct D_Element* elem = dict_get(input_key);

    assert(D_C_FUNC == elem->dtype);
    assert(input_func == elem->u.cfunc);
}


int main() {
    test_dict_put_number();
    test_dict_put_number_same_hash_two_numbers();
    test_dict_put_cfunc();
    test_dict_get_type_number();
    test_dict_get_type_number_same_hash_two_numbers();
    test_dict_get_type_cfunc();

    return 0;
}
