#include "IndentedPrintDecorator.h"

void IndentedPrintDecorator::indent()
{
    if (level<127)
        level++;
}

void IndentedPrintDecorator::unindent()
{
    if (level>0)
        level--;
}

size_t IndentedPrintDecorator::write(uint8_t c)
{
    size_t n = 0;

    if (isNewLine)
        n += writeTabs();

    n += sink.write(c);

    isNewLine = c == '\n';
        
    return n;
}

size_t IndentedPrintDecorator::writeTabs()
{
    size_t n = 0;

    for (int i = 0; i<level; i++)
        n += sink.write(' ');

    return n;
}