#include "StringBuilder.h"


void StringBuilder::append(const char* format, ...)
{
    char* tail = buffer + length;

    va_list args;
    va_start(args, format);
    vsnprintf(tail, capacity - length, format, args);
    va_end(args);

    length += strlen(tail);
}