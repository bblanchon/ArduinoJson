# CAUTION: this file is invoked by https://github.com/google/oss-fuzz

CXXFLAGS += -I../../src -DARDUINOJSON_DEBUG=1

all: \
	$(OUT)/json_fuzzer \
	$(OUT)/json_fuzzer_seed_corpus.zip \
	$(OUT)/json_fuzzer.options \
	$(OUT)/msgpack_fuzzer \
	$(OUT)/msgpack_fuzzer_seed_corpus.zip \
	$(OUT)/msgpack_fuzzer.options

$(OUT)/%_fuzzer: %_fuzzer.cpp $(shell find ../../src -type f)
	$(CXX) $(CXXFLAGS) $< -o$@ $(LIB_FUZZING_ENGINE)

$(OUT)/%_fuzzer_seed_corpus.zip: %_seed_corpus/*
	zip -j $@ $?

$(OUT)/%_fuzzer.options:
	@echo "[libfuzzer]" > $@
	@echo "max_len = 256" >> $@
	@echo "timeout = 10" >> $@
