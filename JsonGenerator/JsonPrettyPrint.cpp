/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonPrettyPrint.h"

using namespace ArduinoJson::Generator;

size_t JsonPrettyPrint::write(uint8_t c)
{
    size_t n = _inString ? handleStringChar(c) : handleMarkupChar(c);    
    _previousChar = c;
    return n;
}

inline size_t JsonPrettyPrint::handleStringChar(uint8_t c)
{
    bool isQuote = c == '"' && _previousChar != '\\';

    if (isQuote) _inString = false;

    return _sink.write(c);
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
    return indentIfNeeded() + _sink.write(c);
}

inline size_t JsonPrettyPrint::handleBlockClose(uint8_t c)
{  
    return unindentIfNeeded() + _sink.write(c);
}

inline size_t JsonPrettyPrint::handleColumn()
{
    return _sink.write(':') + _sink.write(' ');
}

inline size_t JsonPrettyPrint::handleComma() 
{
    return _sink.write(',') + _sink.println();
}

inline size_t JsonPrettyPrint::handleQuoteOpen()
{
    _inString = true;
    return indentIfNeeded() + _sink.write('"');
}

inline size_t JsonPrettyPrint::handleNormalChar(uint8_t c)
{
    return indentIfNeeded() + _sink.write(c);
}

size_t JsonPrettyPrint::indentIfNeeded()
{
    if (!inEmptyBlock()) return 0;

    _sink.indent();
    return _sink.println();
}

size_t JsonPrettyPrint::unindentIfNeeded()
{
    if (inEmptyBlock()) return 0;

    _sink.unindent();
    return _sink.println();
}
