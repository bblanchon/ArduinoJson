#include "IndentedPrint.h"

void IndentedPrint::indent()
{
    if (level<127)
        level++;
}

void IndentedPrint::unindent()
{
    if (level>0)
        level--;
}

size_t IndentedPrint::write(uint8_t c)
{
    size_t n = 0;

    if (isNewLine)
        n += writeTabs();

    n += sink.write(c);

    isNewLine = c == '\n';
        
    return n;
}

size_t IndentedPrint::writeTabs()
{
    size_t n = 0;

    for (int i = 0; i<level; i++)
        n += sink.write(' ');

    return n;
}