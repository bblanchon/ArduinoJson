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
        n = inString ? writeNormalChar(c) : writeOpening(c);        
        break;

    case '}':
    case ']':  
        n = inString ? writeNormalChar(c) : writeClosing(c);
        break;
        
    case ',':
        n = inString ? writeNormalChar(c) : writeComma();
        break;

    case ':':
        n = inString ? writeNormalChar(c) : writeColumn();
        break;

    case '\"':
        n = writeNormalChar(c);

        if (previousChar != '\\')
            inString = !inString;

        break;

    default:
        n = writeNormalChar(c);     
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

size_t PrettyPrintDecorator::writeNormalChar(uint8_t c)
{
    bool inEmptyBlock = !inString && (previousChar == '{' || previousChar == '[');

    return inEmptyBlock ? writeln() + sink.write(c) : sink.write(c);
}

size_t PrettyPrintDecorator::writeColumn()
{
    return sink.write(':') + sink.write(' ');
}

size_t PrettyPrintDecorator::writeComma()
{
    return sink.write(',') + writeln();
}

size_t PrettyPrintDecorator::writeOpening(uint8_t c)
{
    size_t n;
    
    if (previousChar == '{' || previousChar == '[')
    {
        n = writeln() + sink.write(c);
    }
    else
    {
        n = sink.write(c);
    }

    indent++;
    return n;
}

size_t PrettyPrintDecorator::writeClosing(uint8_t c)
{
    indent--;
    if (previousChar == '{' || previousChar == '[')
    {
        return sink.write(c);
    }
    else
    {
        return writeln() + sink.write(c);
    }
}