// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <typename TIterator>
class IteratorReader {
  TIterator _ptr, _end;

 public:
  explicit IteratorReader(TIterator begin, TIterator end)
      : _ptr(begin), _end(end) {}

  int read() {
    if (_ptr < _end)
      return static_cast<unsigned char>(*_ptr++);
    else
      return -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    size_t i = 0;
    while (i < length && _ptr < _end) buffer[i++] = *_ptr++;
    return i;
  }
};

template <typename T>
struct void_ {
  typedef void type;
};

template <typename TSource>
struct Reader<TSource, typename void_<typename TSource::const_iterator>::type>
    : IteratorReader<typename TSource::const_iterator> {
  explicit Reader(const TSource& source)
      : IteratorReader<typename TSource::const_iterator>(source.begin(),
                                                         source.end()) {}
};
}  // namespace ARDUINOJSON_NAMESPACE
