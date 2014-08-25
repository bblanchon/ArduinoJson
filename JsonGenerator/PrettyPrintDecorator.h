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
        isInAString = false;
    }

    virtual size_t write(uint8_t);

private:
    int indent;
    uint8_t previousChar;
    Print& sink;
    bool isInAString;

    size_t writeln();
};

