// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <string>

struct custom_char_traits : std::char_traits<char> {};
struct custom_allocator : std::allocator<char> {};
typedef std::basic_string<char, custom_char_traits, custom_allocator>
    custom_string;
