/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#ifndef ARDUINO

class Print;

class Printable
{
public:

    virtual size_t printTo(Print& p) const = 0;
};

#else

#include <Printable.h>

#endif
