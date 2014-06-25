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

void StringBuilder::appendEscaped(const char* s)
{
    if (length > capacity - 3) return;

    buffer[length++] = '"';

    while (*s && length<capacity-2)
    {
        if (*s == '"')
            buffer[length++] = '\\';

        buffer[length++] = *s;

        s++;
    }

    buffer[length++] = '"';
}