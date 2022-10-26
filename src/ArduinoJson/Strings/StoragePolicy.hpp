// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

struct LinkStringStoragePolicy {};

struct CopyStringStoragePolicy {};

struct LinkOrCopyStringStoragePolicy {
  LinkOrCopyStringStoragePolicy(bool l) : link(l) {}

  bool link;
};

}  // namespace ARDUINOJSON_NAMESPACE
