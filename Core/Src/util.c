#include "util.h"
#include "stdlib.h"

bool
starts_with(const char *str, const char * __restrict pattern)
{
    size_t pat_size = strlen(pattern);
    if (pat_size > strlen(str)) { return false; }

    for (size_t i = 0; i < pat_size; ++i)
    {
        if (str[i] != pattern[i]) { return false; }
    }

    return true;
}

char *
get_after(const char *str, const char delim)
{
    size_t delim_pos = __SIZE_MAX__;
    size_t after_len = 0;
    for (size_t i = 0; i < strlen(str); ++i)
    {
        if (delim_pos != __SIZE_MAX__)
        { ++after_len; }
        if (delim_pos == __SIZE_MAX__ && str[i] == delim)
        { delim_pos = i; }
    }
    
    if (!after_len) { return NULL; }

    char *after = (char *)malloc(after_len+1);
    if  (!after)  { return NULL; }

    strcpy(after, str+delim_pos+1);

    return after;
}

int
get_after_num(const char *str, const char delim)
{
    char *after = get_after(str, delim);
    if (!after) { return -1; }

    int num = atoi(str);
    free(after);

    if (!num) { return -1; }

    return num;
}