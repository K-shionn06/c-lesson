#include "clesson.h"
#include <string.h>

#define MAX_SIZE_INPUT 63

static char input[MAX_SIZE_INPUT + 1] = "123 456";
static int pos = 0;


int cl_getc() {
    if(strlen(input) == pos)
        return EOF;
    return input[pos++];
}

int _my_strcpy(char * dest, char const * src) {
    int i;
    
    if (MAX_SIZE_INPUT < strlen(src))
        puts("The string has been force truncated in _my_strcpy!");

    for (i = 0; i < MAX_SIZE_INPUT; i++) {
        *dest++ = *src++;
        if ('\0' == *src)
            return i + 1;
    }
    dest[MAX_SIZE_INPUT + 1] = '\0';

    return i + 1;
}

void cl_getc_set_src(char *str) {
    int cpied_len;
    cpied_len = _my_strcpy(input, str);

    if (strlen(str) != cpied_len)
        puts("Did not copy correctly!");

    pos = 0;
}
