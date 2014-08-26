/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonPrettyPrint.h"

using namespace ArduinoJson::Generator;

size_t JsonPrettyPrint::write(uint8_t c)
{
    size_t n = inString ? handleStringChar(c) : handleMarkupChar(c);    
    previousChar = c;
    return n;
}

inline size_t JsonPrettyPrint::handleStringChar(uint8_t c)
{
    bool isQuote = c == '"' && previousChar != '\\';

    if (isQuote) inString = false;

    return sink.write(c);
}

inline size_t JsonPrettyPrint::handleMarkupChar(uint8_t c)
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

inline size_t JsonPrettyPrint::handleBlockOpen(uint8_t c)
{
    return indentIfNeeded() + sink.write(c);
}

inline size_t JsonPrettyPrint::handleBlockClose(uint8_t c)
{  
    return unindentIfNeeded() + sink.write(c);
}

inline size_t JsonPrettyPrint::handleColumn()
{
    return sink.write(':') + sink.write(' ');
}

inline size_t JsonPrettyPrint::handleComma() 
{
    return sink.write(',') + sink.println();
}

inline size_t JsonPrettyPrint::handleQuoteOpen()
{
    inString = true;
    return indentIfNeeded() + sink.write('"');
}

inline size_t JsonPrettyPrint::handleNormalChar(uint8_t c)
{
    return indentIfNeeded() + sink.write(c);
}

size_t JsonPrettyPrint::indentIfNeeded()
{
    if (!inEmptyBlock()) return 0;

    sink.indent();
    return sink.println();
}

size_t JsonPrettyPrint::unindentIfNeeded()
{
    if (inEmptyBlock()) return 0;

    sink.unindent();
    return sink.println();
}