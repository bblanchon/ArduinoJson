// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

// This file is NOT use by Google's OSS fuzz
// I only use it to reproduce the bugs found

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

std::string read(const char* path) {
  std::ifstream file(path);
  return std::string(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: msgpack_fuzzer files" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    std::cout << "Loading " << argv[i] << std::endl;
    std::string buffer = read(argv[i]);
    LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t*>(buffer.data()),
                           buffer.size());
  }
  return 0;
}
