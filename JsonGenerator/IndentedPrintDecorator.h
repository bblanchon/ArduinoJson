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
        : sink(p)
    {
        pendingTabCount = 0;
        currentTabCount = 0;
        isNewLine = true;
    }

    void indent();
    void unindent();

    bool isNewLine;
    virtual size_t write(uint8_t);

private:
    Print& sink;
    int currentTabCount;
    int pendingTabCount;

    size_t writeCurrentTabs();
    size_t writeTabs(int count);
};

