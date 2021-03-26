#include "dict.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TABLE_SIZE 1024

struct Node {
    char *key;
    int value;
    struct Node *next;
};

struct Node *array[TABLE_SIZE];


/* functions */


bool streq(const char *s1, const char *s2) {
    if (0 == strcmp(s1, s2))
        return true;
    else
        return false;
}

static int hash(char *str) {
    unsigned int val = 0;
    while (*str) {
        val += *str++;
    }
    return (int)(val % 1024);
}

static struct Node * make_new_node(char *key, int value) {
    struct Node *node;
    node = (struct Node *)malloc(sizeof(struct Node));

    node->key = key;
    node->value = value;

    return node;
}

static struct Node * get_end_node(struct Node *head) {
    struct Node *node;
    node = head;

    while (true) {
        if (NULL == node->next)
            return node;
        else
            node = node->next;
    }
}

static struct Node * search_node_by_key(struct Node *head, char *key) {
    struct Node *node;
    node = head;

    while (NULL != node) {
        if (streq(key, node->key))
            return node;
        else
            node = node->next;
    }

    return NULL;
}

static void update_node(struct Node *head, char *key, int value) {
    struct Node *node;
    node = search_node_by_key(head, key);
    assert(NULL != node);

    node->value = value;
}

static void insert_node(struct Node *head, char *key, int value) {
    struct Node *node;
    node = get_end_node(head);
    assert(node != NULL);

    node->next = make_new_node(key, value);
}

static void update_or_insert_list(int idx, char *key, int value) {
    struct Node *node;
    node = search_node_by_key(array[idx], key);
    assert(NULL != node);

    if (NULL == node) {
        insert_node(array[idx], key, value);
    }
    else {
        update_node(array[idx], key, value);
    }
}

bool dict_key_isused(char *key) {
    int idx = hash(key);

    if (NULL != search_node_by_key(array[idx], key))
        return true;
    else
        return false;
}

void dict_put_number(char *key, int value) {
    int idx = hash(key);

    if (NULL == array[idx]) {
        array[idx] = make_new_node(key, value);
    }
    else {
        update_or_insert_list(idx, key, value);
    }
}

int dict_get_number(char *key) {
    int idx = hash(key);
    struct Node *node;
    node = search_node_by_key(array[idx], key);

    return node->value;
}


/* unit test */


static void clear_array_for_test() {
    // This function only for test

    for (int i = 0; i < TABLE_SIZE; i++) {
        array[i] = NULL;
    }
}

static void assert_1st_node(char *expect_key, int expect_value) {
    int idx = hash(expect_key);

    assert(streq(expect_key, array[idx]->key));
    assert(expect_value == array[idx]->value);
}

static void assert_2nd_node(char *expect_key, int expect_value) {
    int idx = hash(expect_key);

    assert(streq(expect_key, array[idx]->next->key));
    assert(expect_value == array[idx]->next->value);
}

static void test_update_node() {
    char *input_key = "apple";
    int input_idx = hash(input_key);
    int input_number = 2000;
    int expect_number = 3000; 

    clear_array_for_test();
    array[input_idx] = make_new_node(input_key, input_number);

    update_node(array[input_idx], input_key, expect_number);

    assert_1st_node(input_key, expect_number);
}

static void test_insert_node() {
    // This test depends on hash function

    char *input_key1 = "abc";
    char *input_key2 = "cba";
    int input_idx = hash(input_key1);
    int input_number1 = 2000;
    int input_number2 = 3000; 

    clear_array_for_test();
    array[input_idx] = make_new_node(input_key1, input_number1);

    assert(array[input_idx] != NULL);
    insert_node(array[input_idx], input_key2, input_number2);

    assert_2nd_node(input_key2, input_number2);
}

static void test_dict_put_number() {
    char *input_key = "yellow";
    int input_number = 2000;

    clear_array_for_test();
    dict_put_number(input_key, input_number);

    assert_1st_node(input_key, input_number);
}

static void test_dict_get_number() {
    char *input_key = "yellow";
    int input_number = 2000;

    clear_array_for_test();
    dict_put_number(input_key, input_number);

    int actual_number = dict_get_number(input_key);

    assert(input_number == actual_number);
}


#if 0
int main() {
    test_insert_node();
    test_update_node();
    test_dict_get_number();
    test_dict_put_number();

    return 0;
}
#endif
