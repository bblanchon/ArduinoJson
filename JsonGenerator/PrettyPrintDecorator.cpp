/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "PrettyPrintDecorator.h"

using namespace ArduinoJson::Generator;

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
        return handleBlockOpen(c);

    case '}':
    case ']':
        return handleBlockClose(c);

    case ':':
        return handleColumn();

    case ',':
        return handleComma();

    case '"':
        return handleQuoteOpen();

    default:
        return handleNormalChar(c);
    }
}

size_t PrettyPrintDecorator::handleBlockOpen(uint8_t c)
{
    return indentIfNeeded() + sink.write(c);
}

size_t PrettyPrintDecorator::handleBlockClose(uint8_t c)
{  
    return unindentIfNeeded() + sink.write(c);
}

size_t PrettyPrintDecorator::handleColumn()
{
    return sink.write(':') + sink.write(' ');
}

size_t PrettyPrintDecorator::handleComma()
{
    return sink.write(',') + sink.println();
}

size_t PrettyPrintDecorator::handleQuoteOpen()
{
    inString = true;
    return indentIfNeeded() + sink.write('"');
}

size_t PrettyPrintDecorator::handleNormalChar(uint8_t c)
{
    return indentIfNeeded() + sink.write(c);
}

size_t PrettyPrintDecorator::indentIfNeeded()
{
    if (!inEmptyBlock()) return 0;

    sink.indent();
    return sink.println();
}

size_t PrettyPrintDecorator::unindentIfNeeded()
{
    if (inEmptyBlock()) return 0;

    sink.unindent();
    return sink.println();
}