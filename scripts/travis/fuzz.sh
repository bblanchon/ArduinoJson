#!/bin/bash -eux

ROOT_DIR=$(dirname $0)/../../
INCLUDE_DIR=$ROOT_DIR/src/
FUZZING_DIR=$ROOT_DIR/fuzzing/
JSON_CORPUS_DIR=$FUZZING_DIR/my_corpus
JSON_SEED_CORPUS_DIR=$FUZZING_DIR/seed_corpus

CXX="clang++-$CLANG"
CXXFLAGS="-g -fprofile-instr-generate -fcoverage-mapping -fsanitize=address,fuzzer"

$CXX $CXXFLAGS -o json_fuzzer -I$INCLUDE_DIR $FUZZING_DIR/fuzzer.cpp

export ASAN_OPTIONS="detect_leaks=0"
export LLVM_PROFILE_FILE="json_fuzzer.profraw"
./json_fuzzer "$JSON_CORPUS_DIR" "$JSON_SEED_CORPUS_DIR" -max_total_time=60

llvm-profdata-$CLANG merge -sparse json_fuzzer.profraw -o json_fuzzer.profdata

llvm-cov-$CLANG report ./json_fuzzer -instr-profile=json_fuzzer.profdata
