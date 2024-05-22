// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <istream>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TSource>
struct Reader<TSource, enable_if_t<is_base_of<std::istream, TSource>::value>> {
 public:
  explicit Reader(std::istream& stream) : stream_(&stream) {}

  int read() {
    return stream_->get();
  }

  size_t readBytes(char* buffer, size_t length) {
    stream_->read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(stream_->gcount());
  }

 private:
  std::istream* stream_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
