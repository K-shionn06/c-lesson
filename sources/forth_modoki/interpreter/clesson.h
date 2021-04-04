#include <stdio.h>

#ifndef CLESSON_H
#define CLESSON_H

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();
void cl_getc_set_src(char* str);

#endif
