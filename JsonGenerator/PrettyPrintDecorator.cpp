/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "PrettyPrintDecorator.h"

size_t PrettyPrintDecorator::write(uint8_t c)
{
    size_t n;

    switch (c)
    {
    case '{':
    case '[':              

        if (previousChar == '{' || previousChar == '[')
        {
            n = writeln() + sink.write(c);
        }
        else
        {
            n = sink.write(c);
        }

        indent++;
        break;

    case '}':
    case ']':
        indent--;

        if (previousChar == '{' || previousChar == '[')
        {
            n = sink.write(c);
        }
        else
        {
            n = writeln() + sink.write(c);
        }
        break;
        
    case ',':
        if (isInAString)
        {
            n = sink.write(c);
        }
        else
        {
            n = sink.write(c) + writeln();
        }
        break;

    case ':':
        if (isInAString)
        {
            n = sink.write(c);
        }
        else
        {
            n = sink.write(c) + sink.write(' ');
        }
        break;

    case '\"':
        if (previousChar != '\\')
        {
            isInAString = !isInAString;
        }
        // no break;

    default:

        if (previousChar == '{' || previousChar == '[')
        {
            n = writeln() + sink.write(c);
        }
        else
        {
            n = sink.write(c);
        }

        break;
    }

    previousChar = c;
    return n;
}

size_t PrettyPrintDecorator::writeln()
{
    size_t n = sink.write('\n');

    for (int i = 0; i < indent; i++)
        n += sink.write(' ');

    return n;
}