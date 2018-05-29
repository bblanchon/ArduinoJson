// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TJsonBuffer>
class StringCopier {
 public:
  StringCopier(TJsonBuffer& jb) : _jb(&jb) {}

  typedef typename TJsonBuffer::String String;

  String startString() {
    return _jb->startString();
  }

 private:
  TJsonBuffer* _jb;
};
}  // namespace Internals
}  // namespace ArduinoJson
