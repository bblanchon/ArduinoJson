# CAUTION: this file is invoked by https://github.com/google/oss-fuzz

CXXFLAGS += -I../../src -DARDUINOJSON_DEBUG=1 -std=c++11

all: \
	$(OUT)/json_fuzzer \
	$(OUT)/json_fuzzer_seed_corpus.zip \
	$(OUT)/json_fuzzer.options \
	$(OUT)/msgpack_fuzzer \
	$(OUT)/msgpack_fuzzer_seed_corpus.zip \
	$(OUT)/msgpack_fuzzer.options \
	$(OUT)/msgpack_fuzzer.dict \
	$(OUT)/number_fuzzer \
	$(OUT)/number_fuzzer_seed_corpus.zip \
	$(OUT)/number_fuzzer.options

$(OUT)/%_fuzzer: %_fuzzer.cpp $(shell find ../../src -type f)
	$(CXX) $(CXXFLAGS) $< -o$@ $(LIB_FUZZING_ENGINE)

$(OUT)/%_fuzzer_seed_corpus.zip: %_seed_corpus/*
	zip -j $@ $?

$(OUT)/%_fuzzer.options:
	@echo "[libfuzzer]" > $@
	@echo "max_len = 4096" >> $@
	@echo "timeout = 10" >> $@

# Copy a per-target dictionary into $OUT when one exists, so OSS-Fuzz
# (and local runs with OUT=...) pick it up automatically.
$(OUT)/%_fuzzer.dict: %_fuzzer.dict
	cp $< $@
