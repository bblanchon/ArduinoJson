#!/bin/bash -eux

ROOT_DIR=$(dirname $0)/../../
INCLUDE_DIR=${ROOT_DIR}/src/
FUZZING_DIR=${ROOT_DIR}/fuzzing/
CXXFLAGS="-g -fprofile-instr-generate -fcoverage-mapping -fsanitize=address,fuzzer"

fuzz() {
	NAME="$1"
	FUZZER="${NAME}_fuzzer"
	FUZZER_CPP="${FUZZING_DIR}/${NAME}_fuzzer.cpp"
	CORPUS_DIR="${FUZZING_DIR}/${NAME}_corpus"
	SEED_CORPUS_DIR="${FUZZING_DIR}/${NAME}_seed_corpus"

	clang++-${CLANG} ${CXXFLAGS} -o ${FUZZER} -I$INCLUDE_DIR ${FUZZER_CPP}

	export ASAN_OPTIONS="detect_leaks=0"
	export LLVM_PROFILE_FILE="${FUZZER}.profraw"
	./${FUZZER} "$CORPUS_DIR" "$SEED_CORPUS_DIR" -max_total_time=30 -timeout=1

	llvm-profdata-${CLANG} merge -sparse ${LLVM_PROFILE_FILE} -o ${FUZZER}.profdata
	llvm-cov-${CLANG} report ./${FUZZER} -instr-profile=${FUZZER}.profdata
}

fuzz json
fuzz msgpack
