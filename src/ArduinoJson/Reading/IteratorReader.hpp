// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TIterator>
class IteratorReader {
  TIterator _ptr, _end;

 public:
  explicit IteratorReader(TIterator begin, TIterator end)
      : _ptr(begin), _end(end) {}

  bool ended() const {
    return _ptr == _end;
  }

  char read() {
    return char(*_ptr++);
  }
};

template <typename TInput>
inline IteratorReader<typename TInput::const_iterator> makeReader(
    const TInput& input) {
  return IteratorReader<typename TInput::const_iterator>(input.begin(),
                                                         input.end());
}
}  // namespace Internals
}  // namespace ArduinoJson
