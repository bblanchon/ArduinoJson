// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Serialization/measure.hpp"
#include "../Serialization/serialize.hpp"
#include "./IndentedPrint.hpp"
#include "./JsonSerializer.hpp"
#include "./Prettyfier.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TPrint>
class PrettyJsonSerializer_Base {
 public:
  PrettyJsonSerializer_Base(TPrint &output)
      : _indentedPrint(output), _prettyfier(_indentedPrint) {}

 protected:
  IndentedPrint<TPrint> _indentedPrint;
  Prettyfier<TPrint> _prettyfier;
};

template <typename TPrint>
class PrettyJsonSerializer : PrettyJsonSerializer_Base<TPrint>,
                             public JsonSerializer<Prettyfier<TPrint> > {
 public:
  PrettyJsonSerializer(TPrint &output)
      : PrettyJsonSerializer_Base<TPrint>(output),
        JsonSerializer<Prettyfier<TPrint> >(
            PrettyJsonSerializer_Base<TPrint>::_prettyfier) {}
};
}  // namespace Internals

template <typename TSource, typename TDestination>
size_t serializeJsonPretty(TSource &source, TDestination &destination) {
  using namespace Internals;
  return serialize<PrettyJsonSerializer>(source, destination);
}

template <typename TSource>
size_t serializeJsonPretty(const TSource &source, char *buffer,
                           size_t bufferSize) {
  using namespace Internals;
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}

template <typename TSource>
size_t measureJsonPretty(const TSource &source) {
  using namespace Internals;
  return measure<PrettyJsonSerializer>(source);
}

}  // namespace ArduinoJson
