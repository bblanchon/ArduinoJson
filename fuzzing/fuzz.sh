#!/bin/bash
# This script mimics an invocation from https://github.com/google/oss-fuzz

cd $(dirname $0)
export CXX='clang++'
export CXXFLAGS='-fsanitize-coverage=trace-pc-guard -fsanitize=address'
export LIB_FUZZING_ENGINE=-lFuzzer
make OUT=.
./json_fuzzer my_corpus seed_corpus -max_len=1024 -timeout=10
