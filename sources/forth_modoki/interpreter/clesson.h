#include <stdio.h>

#ifndef CLESSON_H
#define CLESSON_H

/*
return one character and move cursor.
return EOF if end of file.

if fp is not NULL, cl_getc returns a char from the file.
*/
int cl_getc();
void cl_getc_set_src(char* str);

void cl_getc_attach_file(char* file_name);
void cl_getc_detach_file();

#endif
