#pragma once
/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"

class PrettyPrintDecorator : public Print
{
public:

    PrettyPrintDecorator(Print& p)
        : indent(0), sink(p)
    {
        previousChar = 0;
        inString = false;
    }

    virtual size_t write(uint8_t);

private:
    int indent;
    uint8_t previousChar;
    Print& sink;
    bool inString;

    bool inEmptyBlock()
    {
        return previousChar == '{' || previousChar == '[';
    }
    
    size_t handleStringChar(uint8_t);
    size_t handleMarkupChar(uint8_t);

    size_t handleBlockClose(uint8_t);
    size_t handleBlockOpen(uint8_t);
    size_t handleColumn();
    size_t handleComma();
    size_t handleQuoteOpen();
    size_t handleNormalChar(uint8_t);

    size_t breakAndIndent();

    size_t breakThenWrite(uint8_t c)
    {
        return breakAndIndent() + writeChar(c);
    }

    size_t writeThenBreak(uint8_t c)
    {
        return writeChar(c) + breakAndIndent();
    }

    size_t writeChar(uint8_t c)
    {
        return sink.write(c);
    }
};

