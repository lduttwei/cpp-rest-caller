#ifndef PARSER_UTILITY_HPP
#define PARSER_UTILITY_HPP

#include <iostream>
#include <string.h>

inline int cmp_arg(const char* arg, const char* cmp)
{
	return strcmp(arg, cmp) == 0;
}

#endif
