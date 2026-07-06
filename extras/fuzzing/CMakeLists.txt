# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2026, Benoit BLANCHON
# MIT License

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

function(add_fuzzer name)
	set(FUZZER "${name}_fuzzer")
	set(REPRODUCER "${FUZZER}_reproducer")
	set(CORPUS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${name}_corpus")
	set(SEED_CORPUS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${name}_seed_corpus")

	add_executable("${FUZZER}"
		"${name}_fuzzer.cpp"
	)
	target_link_libraries("${FUZZER}"
		ArduinoJson
	)
	set_target_properties("${FUZZER}"
		PROPERTIES
			COMPILE_FLAGS "-fprofile-instr-generate -fcoverage-mapping -fsanitize=fuzzer -fno-sanitize-recover=all"
			LINK_FLAGS "-fprofile-instr-generate -fcoverage-mapping -fsanitize=fuzzer -fno-sanitize-recover=all"
	)

	add_test(
		NAME "${FUZZER}"
		COMMAND "${FUZZER}" "${CORPUS_DIR}" "${SEED_CORPUS_DIR}" -max_total_time=5 -timeout=1
	)
	set_tests_properties("${FUZZER}"
		PROPERTIES
		LABELS "Fuzzing"
	)

	add_executable("${REPRODUCER}"
		"${name}_fuzzer.cpp"
		reproducer.cpp
	)
	target_link_libraries("${REPRODUCER}"
		ArduinoJson
	)
endfunction()

# Needs Clang 6+ to compile
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6)
	if(DEFINED ENV{GITHUB_ACTIONS} AND CMAKE_CXX_COMPILER_VERSION MATCHES "^11\\.")
		# Clang 11 fails on GitHub Actions with the following error:
		# > ERROR: UndefinedBehaviorSanitizer failed to allocate 0x0 (0) bytes of SetAlternateSignalStack (error code: 22)
		# > Sanitizer CHECK failed: /build/llvm-toolchain-11-mnvtwk/llvm-toolchain-11-11.1.0/compiler-rt/lib/sanitizer_common/sanitizer_common.cpp:54 ((0 && "unable to mmap")) != (0) (0, 0)
		message(WARNING "Fuzzing is disabled on GitHub Actions to workaround a bug in Clang 11")
		return()
	endif()

	add_fuzzer(json)
	add_fuzzer(msgpack)
	add_fuzzer(number)
endif()
