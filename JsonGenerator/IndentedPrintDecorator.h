#pragma once
/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"

class IndentedPrintDecorator : public Print
{
public:

    IndentedPrintDecorator(Print& p)
        : indent(0), sink(p)
    {
    }

    virtual size_t write(uint8_t);

private:
    int indent;
    Print& sink;

    size_t writeln();
};

