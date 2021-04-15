#include "exec_array.h"

#ifndef CONTINUATION_H
#define CONTINUATION_H  

struct Continuation {
    struct EA_ElementArray* byte_codes;
    unsigned int pc;
};

#endif
