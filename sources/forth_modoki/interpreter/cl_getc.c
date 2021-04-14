#include "clesson.h"
#include <string.h>

static const char* input = "123 456";
static FILE* fp = NULL;
static int pos = 0;


int cl_getc() {
    if (NULL != fp) {
        return fgetc(fp);
    }

    if(strlen(input) == pos)
        return EOF;
    return input[pos++];
}

void cl_getc_set_src(char* str){
    input = str;
    pos = 0;
}

void cl_getc_attach_file(char* file_name) {
    fp = fopen(file_name, "r");
}

void cl_getc_detach_file() {
    fclose(fp);
    fp = NULL;
}
