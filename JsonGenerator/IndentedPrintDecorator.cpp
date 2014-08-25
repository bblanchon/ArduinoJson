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
        previousChar = c;
        return sink.write(c);

    case '}':
        indent--;

        if (previousChar == '{')
        {
            previousChar = c;
            return sink.write(c);
        }
        else
        {
            previousChar = c;
            return writeln() + sink.write(c);
        }
        
    case ',':
        previousChar = c;
        if (isInAString)
        {
            return sink.write(c);
        }
        else
        {
            return sink.write(c) + writeln();
        }

    case ':':
        previousChar = c;
        if (isInAString)
        {
            return sink.write(c);
        }
        else
        {
            return sink.write(c) + sink.write(' ');
        }

    case '\"':
        if (previousChar != '\\')
        {
            isInAString = !isInAString;
        }

    default:

        if (previousChar == '{')
        {
            previousChar = c;
            return writeln() + sink.write(c);
        }
        else
        {
            previousChar = c;
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