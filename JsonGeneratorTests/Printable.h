/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

class Print;

class Printable
{
public:

    virtual size_t writeTo(Print& p) const = 0;
};

