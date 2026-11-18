FROM ubuntu:26.04

RUN apt-get update \
 && apt-get install -y --no-install-recommends cmake git ninja-build clang-21 clang-tidy-21 clang-format-21 libc++-21-dev libc++abi-21-dev \
 && rm -rf /var/lib/apt/lists/*

RUN for f in /usr/lib/llvm-21/bin/*; do \
      update-alternatives --install /usr/bin/$(basename $f) $(basename $f) $f 100; \
    done

ENV CC=clang CXX=clang++
