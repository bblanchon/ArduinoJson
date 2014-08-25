/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "IndentedPrintDecorator.h"

size_t IndentedPrintDecorator::write(uint8_t c)
{
    return sink.write(c);
}