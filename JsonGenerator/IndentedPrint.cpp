#include "IndentedPrint.h"

using namespace ArduinoJson::Generator;

void IndentedPrint::indent()
{
    if (_level < MAX_LEVEL)
        _level++;
}

void IndentedPrint::unindent()
{
    if (_level > 0)
        _level--;
}

void IndentedPrint::setTabSize(uint8_t n)
{
    if (n < MAX_TAB_SIZE)
        _tabSize = n;
}

size_t IndentedPrint::write(uint8_t c)
{
    size_t n = 0;

    if (_isNewLine)
        n += writeTabs();

    n += _sink.write(c);

    _isNewLine = c == '\n';

    return n;
}

inline size_t IndentedPrint::writeTabs()
{
    size_t n = 0;

    for (int i = 0; i < _level*_tabSize; i++)
        n += _sink.write(' ');

    return n;
}
