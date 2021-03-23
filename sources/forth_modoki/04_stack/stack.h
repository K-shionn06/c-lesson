enum StackDataType {
    NUMBER,
    NAME
};

union StackData {
    int number;
    char *name;
};

typedef struct {
    enum StackDataType dtype;
    union StackData u;
} stack_data_t;

void stack_clear();

bool stack_isfull();
bool stack_isempty();

bool stack_push_number(int);
bool stack_push_string(char *);
bool stack_pop(stack_data_t *);
