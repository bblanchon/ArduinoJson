// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
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
};

template <typename TInput>
inline IteratorReader<typename TInput::const_iterator> makeReader(
    const TInput& input) {
  return IteratorReader<typename TInput::const_iterator>(input.begin(),
                                                         input.end());
}
}  // namespace ARDUINOJSON_NAMESPACE
