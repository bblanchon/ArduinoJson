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

    size_t writeln();

    size_t handleStringChar(uint8_t);
    size_t handleMarkupChar(uint8_t);

    size_t writeClosing(uint8_t);
    size_t writeColumn();
    size_t writeComma();
    size_t writeValueChar(uint8_t);
    size_t writeOpening(uint8_t);
    size_t writeQuote();
};

