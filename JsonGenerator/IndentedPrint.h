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
                isNewLine = true;
            }

            virtual size_t write(uint8_t);

            void indent();
            void unindent();

        private:
            Print& sink;
            uint8_t level : 7;
            bool isNewLine : 1;

            size_t writeTabs();
        };
    }
}

