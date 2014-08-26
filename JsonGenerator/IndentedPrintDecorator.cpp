#include "IndentedPrintDecorator.h"

void IndentedPrintDecorator::indent()
{
    currentTabCount++;
}

void IndentedPrintDecorator::unindent()
{
    currentTabCount--;
}

size_t IndentedPrintDecorator::write(uint8_t c)
{
    size_t n = 0;

    if (isNewLine)
        n += writeCurrentTabs();

    n += sink.write(c);

    isNewLine = c == '\n';
        
    return n;
}

size_t IndentedPrintDecorator::writeCurrentTabs()
{
    return writeTabs(currentTabCount);
}

size_t IndentedPrintDecorator::writeTabs(int count)
{
    size_t n = 0;

    for (int i = 0; i<count; i++)
        n += sink.write(' ');

    return n;
}