// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <string>

struct custom_char_traits : std::char_traits<char> {};

using custom_string = std::basic_string<char, custom_char_traits>;
