#ifndef __UTIL_H__
#define __UTIL_H__

#include "string.h"
#include "stdbool.h"

bool
starts_with(const char *str, const char * __restrict pattern);

char *
get_after(const char *str, const char delim);

int
get_after_num(const char *str, const char delim);

#endif /* __UTIL_H__ */