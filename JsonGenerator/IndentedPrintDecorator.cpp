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
        emptyBlock = true;
        return sink.write(c);

    case '}':
        indent--;

        if (emptyBlock)
        {
            return sink.write(c);
        }
        else
        {
            return writeln() + sink.write(c);
        }

    default:

        if (emptyBlock)
        {
            emptyBlock = false;
            return writeln() + sink.write(c);
        }
        else
        {
            return sink.write(c);
        }
    }
}

size_t IndentedPrintDecorator::writeln()
{
    size_t n = sink.write('\n');

    for (int i = 0; i < indent; i++)
        n += sink.write(' ');

    return n;
}