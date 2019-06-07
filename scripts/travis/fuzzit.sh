#!/bin/bash -eux

ROOT_DIR=$(dirname $0)/../../
INCLUDE_DIR=${ROOT_DIR}/src/
FUZZING_DIR=${ROOT_DIR}/fuzzing/
CXXFLAGS="-g -fprofile-instr-generate -fcoverage-mapping -fsanitize=address,undefined,fuzzer -fno-sanitize-recover=all"

fuzz() {
	FUZZER_NAME="$1"
	TARGET_NAME="$2"
	FUZZER="${NAME}_fuzzer"
	FUZZER_CPP="${FUZZING_DIR}/${FUZZER_NAME}_fuzzer.cpp"
	CORPUS_DIR="${FUZZING_DIR}/${FUZZER_NAME}_corpus"
	SEED_CORPUS_DIR="${FUZZING_DIR}/${FUZZER_NAME}_seed_corpus"

	clang++-${CLANG} ${CXXFLAGS} -o ${FUZZER} -I$INCLUDE_DIR ${FUZZER_CPP}

	export ASAN_OPTIONS="detect_leaks=0"
	export LLVM_PROFILE_FILE="${FUZZER}.profraw"
	if [ "$TRAVIS_PULL_REQUEST" = "false" ]; then
		./fuzzit create job --type fuzzing --args "-detect_leaks=0" arduinojson/${TARGET_NAME} ${FUZZER}
	else
		./fuzzit create job --type local-regression --args "-detect_leaks=0" arduinojson/${TARGET_NAME} ${FUZZER}
	fi

}

wget -O fuzzit https://github.com/fuzzitdev/fuzzit/releases/download/v2.4.35/fuzzit_Linux_x86_64
chmod a+x fuzzit
fuzz json json
fuzz msgpack msgpack

