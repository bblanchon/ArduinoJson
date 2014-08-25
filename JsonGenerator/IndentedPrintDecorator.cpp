/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "IndentedPrintDecorator.h"

size_t IndentedPrintDecorator::write(uint8_t c)
{
    switch (c)
    {
    case '{':
        indent++;        
        return sink.write(c) + writeln();

    case '}':
        indent--;
        return writeln() + sink.write(c);

    default:
        return sink.write(c);
    }
}

size_t IndentedPrintDecorator::writeln()
{
    size_t n = sink.write('\n');

    for (int i = 0; i < indent; i++)
        n += sink.write(' ');

    return n;
}