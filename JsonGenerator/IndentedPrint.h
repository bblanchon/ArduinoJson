/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class IndentedPrint : public Print
        {
        public:

            IndentedPrint(Print& p)
                : sink(p)
            {
                level = 0;
                tabSize = 2;
                isNewLine = true;
            }

            virtual size_t write(uint8_t);

            void indent();
            void unindent();
            void setTabSize(uint8_t n);

        private:
            Print& sink;
            uint8_t level : 4;
            uint8_t tabSize : 3;
            bool isNewLine : 1;
            
            size_t writeTabs();

            const int MAX_LEVEL = 15; // because it's only 4 bits
            const int MAX_TAB_SIZE = 7; // because it's only 3 bits
        };
    }
}

