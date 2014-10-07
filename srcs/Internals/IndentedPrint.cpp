#include "IndentedPrint.h"

using namespace ArduinoJson::Generator;

void IndentedPrint::indent()
{
    if (level < MAX_LEVEL)
        level++;
}

void IndentedPrint::unindent()
{
    if (level > 0)
        level--;
}

void IndentedPrint::setTabSize(uint8_t n)
{
    if (n < MAX_TAB_SIZE)
        tabSize = n;
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

inline size_t IndentedPrint::writeTabs()
{
    size_t n = 0;

    for (int i = 0; i < level*tabSize; i++)
        n += sink.write(' ');

    return n;
}