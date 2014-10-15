/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#ifndef ARDUINO

#include <stddef.h>
#include <stdint.h>

// This class reproduces Arduino's Print
class Print
{
public:

    virtual size_t write(uint8_t) = 0;

    size_t print(const char[]);
    size_t print(double, int = 2);
    size_t print(long);
    size_t println();
};

#else

#include <Print.h>

#endif
