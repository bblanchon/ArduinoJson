// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

// This file is NOT use by Google's OSS fuzz
// I only use it to reproduce the bugs found

#include <stdint.h>  // size_t
#include <stdio.h>   // fopen et al.
#include <stdlib.h>  // exit
#include <iostream>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

std::vector<uint8_t> read(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f) {
    std::cerr << "Failed to open " << path << std::endl;
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  size_t size = static_cast<size_t>(ftell(f));
  fseek(f, 0, SEEK_SET);

  std::vector<uint8_t> buffer(size);
  if (fread(buffer.data(), 1, size, f) != size) {
    fclose(f);
    std::cerr << "Failed to read " << path << std::endl;
    exit(1);
  }

  fclose(f);
  return buffer;
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: msgpack_fuzzer files" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    std::cout << "Loading " << argv[i] << std::endl;
    std::vector<uint8_t> buffer = read(argv[i]);
    LLVMFuzzerTestOneInput(buffer.data(), buffer.size());
  }
  return 0;
}
