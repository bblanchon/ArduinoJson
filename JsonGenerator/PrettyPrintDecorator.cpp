/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "PrettyPrintDecorator.h"

size_t PrettyPrintDecorator::write(uint8_t c)
{
    size_t n = inString ? handleStringChar(c) : handleMarkupChar(c);    
    previousChar = c;
    return n;
}

size_t PrettyPrintDecorator::handleStringChar(uint8_t c)
{
    bool isQuote = c == '"' && previousChar != '\\';

    if (isQuote) inString = false;

    return sink.write(c);
}

size_t PrettyPrintDecorator::handleMarkupChar(uint8_t c)
{
    switch (c)
    {
    case '{':
    case '[':
        return writeOpening(c);

    case '}':
    case ']':
        return writeClosing(c);

    case ',':
        return writeComma();

    case ':':
        return writeColumn();

    case '"':
        return writeQuote();

    default:
        return writeValueChar(c);
    }
}

size_t PrettyPrintDecorator::writeValueChar(uint8_t c)
{
    return inEmptyBlock() ? writeLineBreak() + sink.write(c) : sink.write(c);
}

size_t PrettyPrintDecorator::writeColumn()
{
    return sink.write(':') + sink.write(' ');
}

size_t PrettyPrintDecorator::writeComma()
{
    return sink.write(',') + writeLineBreak();
}

size_t PrettyPrintDecorator::writeOpening(uint8_t c)
{
    size_t n = inEmptyBlock() ? writeLineBreak() + sink.write(c) : sink.write(c);

    indent++;

    return n;
}

size_t PrettyPrintDecorator::writeQuote()
{
    size_t n = inEmptyBlock() ? writeLineBreak() + sink.write('"') : sink.write('"');

    inString = true;

    return n;
}

size_t PrettyPrintDecorator::writeClosing(uint8_t c)
{
    indent--;

    return inEmptyBlock() ? sink.write(c) : writeLineBreak() + sink.write(c);
}

size_t PrettyPrintDecorator::writeLineBreak()
{
    size_t n = sink.write('\n');

    for (int i = 0; i < indent; i++)
        n += sink.write(' ');

    return n;
}