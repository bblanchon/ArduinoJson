// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

namespace StringStoragePolicy {

struct Link {};
struct Copy {};
struct LinkOrCopy {
  bool link;
};
}  // namespace StringStoragePolicy

}  // namespace ARDUINOJSON_NAMESPACE
