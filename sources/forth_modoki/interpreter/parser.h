enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY, 
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

int parse_one(int, struct Token*);
