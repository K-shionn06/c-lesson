#include "dict.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


#define DICT_SIZE 1024

typedef struct {
    char *key;
    dict_value_t value;
} dict_elem_t;

static int dict_pos = 0;
static dict_elem_t dict_array[DICT_SIZE];

bool streq(const char *s1, const char *s2) {
    if (0 == strcmp(s1, s2))
        return true;
    else
        return false;
}

/* core functions */


bool dict_key_isused(char *key) {
    for (int i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key))
            return true;
    }
    return false;
}

void dict_array_set_key(int idx, char *key) {
    dict_array[idx].key = key;
}

void dict_array_set_value(int idx, dict_value_t *value) {
    assert( idx <= dict_pos );

    switch (value->dtype) {
        case S_NUMBER:
            dict_array[idx].value.dtype = value->dtype;
            dict_array[idx].value.u.number = value->u.number;
            break;
        case S_EXE_NAME:
        case S_LIT_NAME:
            dict_array[idx].value.dtype = value->dtype;
            dict_array[idx].value.u.name = value->u.name;
            break;
    }
}

void dict_reassign(char *key, dict_value_t *value) {
    assert( dict_key_isused(key) );

    for (int i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key)) {
            dict_array_set_value(i, value);
        }
    }
}

void dict_put(char *key, dict_value_t *value) {
    assert(dict_pos < DICT_SIZE);

    if (dict_key_isused(key)) {
        dict_reassign(key, value);
    }
    else {
        dict_array_set_key(dict_pos, key);
        dict_array_set_value(dict_pos, value);
        dict_pos++;
    }
}

void dict_array_copy_value(int idx, dict_value_t *out_value) {
        out_value->dtype = dict_array[idx].value.dtype;

        switch (out_value->dtype) {
            case S_NUMBER:
                out_value->u.number = dict_array[idx].value.u.number;
                break;
            case S_EXE_NAME:
            case S_LIT_NAME:
                out_value->u.name = dict_array[idx].value.u.name;
                break;
        }
}

bool dict_get(char *key, dict_value_t *out_value) {
    for (int i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key)) {
            dict_array_copy_value(i, out_value);
            return true;
        }
    }
    return false;
}

void dict_put_number(char *key, int data) {
    dict_value_t value;
    value.dtype = S_NUMBER;
    value.u.number = data;

    dict_put(key, &value);
}

int dict_get_number(char *key) {
    dict_value_t value;

    bool succ = dict_get(key, &value);

    assert(succ);
    assert(S_NUMBER == value.dtype);

    return value.u.number;
}

void dict_print_all() {
    puts("------------------------------------");
    puts("|    key    |    dtype    |    u    ");
    puts("------------------------------------");
    for (int i = 0; i < dict_pos; i++) {
        switch (dict_array[i].value.dtype) {
            case S_NUMBER:
                printf("| %s | %d | %d\n", dict_array[i].key,
                                         dict_array[i].value.dtype,
                                         dict_array[i].value.u.number);
                break;
            case S_EXE_NAME:
            case S_LIT_NAME:
                printf("| %s | %d | %s\n", dict_array[i].key,
                                         dict_array[i].value.dtype,
                                         dict_array[i].value.u.name);
                break;
        }
    }
    puts("------------------------------------");
};


/* UnitTests */


static void assert_value_number(int expect, dict_value_t *actual) {
    assert(S_NUMBER == actual->dtype);
    assert(expect == actual->u.number);
}

static void test_dict_put_new_key() {
    char *input_key = "banana";
    dict_value_t input_value = {
        S_NUMBER,
        {200}
    };

    dict_pos = 0;
    dict_put(input_key, &input_value);

    assert(input_key == dict_array[0].key);
    assert_value_number(input_value.u.number, &dict_array[0].value);
}

static void test_dict_put_used_key() {
    char *input_key = "blue";
    dict_value_t input_value1 = {
        S_NUMBER,
        {300}
    };
    dict_value_t input_value2 = {
        S_NUMBER,
        {2100}
    };

    dict_pos = 0;
    dict_put(input_key, &input_value1);
    dict_put(input_key, &input_value2);

    assert_value_number(input_value2.u.number, &dict_array[0].value);
}

static void test_dict_get_elem() {
    char *input_key = "yellow";
    dict_value_t input_value = {
        S_NUMBER,
        {4300}
    };
    dict_value_t out_value;

    dict_pos = 0;
    dict_put(input_key, &input_value);
    bool succ = dict_get(input_key, &out_value);
    
    assert(succ);
    assert_value_number(input_value.u.number, &out_value);
}

static void test_dict_put_number_new_key() {
    char *input_key = "golf";
    int input_number = 2000;

    dict_pos = 0;
    dict_put_number(input_key, input_number);

    dict_value_t out_value;
    bool succ = dict_get(input_key, &out_value);

    assert(succ);
    assert(S_NUMBER == out_value.dtype);
    assert(input_number == out_value.u.number);
}

static void test_dict_get_number() {
    char *input_key = "pink";
    int input_number = 3000;

    dict_pos = 0;
    dict_put_number(input_key, input_number);
    int actual_number = dict_get_number(input_key);

    assert(input_number == actual_number);
}

int main() {
    test_dict_put_new_key();
    test_dict_put_used_key();
    test_dict_put_number_new_key();
    test_dict_get_elem();
    test_dict_get_number();

    dict_print_all();
    return 0;
}
