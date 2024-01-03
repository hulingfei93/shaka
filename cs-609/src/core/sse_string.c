#include <sse_core.h>

sse_int_t
sse_str_strcmp(sse_str_t *str1, sse_str_t *str2)
{
    size_t min_len;
    sse_int_t result;

    min_len = sse_min(str1->len, str2->len);
    result = sse_memcmp(str1->data, str2->data, min_len);

    if (result != 0) {
        return result;
    }
    
    /* result == 0 */
    if (str1->len == str2->len) {
        return 0;
    }
    else if (str1->len > str2->len) {
        return 1;
    }
    else {
        return -1;
    }
}

