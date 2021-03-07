#include <stdio.h>
#include <assert.h>

static const char* const input = "123 456  1203";


int main() {
    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    // write something here.

    int count = 0;
    const char *p = input;

    do {
        if (' ' == *p) {
            while (' ' != *p++);
            count++;
        }
        if ('0' <= *p && '9' >= *p) {
            switch (count) {
                case 0:
                    answer1 = (*p - 48) + (answer1 * 10);
                    break;
                case 1:
                    answer2 = (*p - 48) + (answer2 * 10);
                    break;
                case 2:
                    answer3 = (*p - 48) + (answer3 * 10);
                    break;
            }
        }
    } while ('\0' != *p++);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 0;
}
