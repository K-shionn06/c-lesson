#include <stdbool.h>
#include <string.h>

bool streq(const char* s1, const char* s2) {
    if (0 == strcmp(s1, s2))
        return true;
    else
        return false;
}
