all: example fuzzer example_stringkeys

example: example.o robinhoodhash.h

example_stringkeys: example_stringkeys.o robinhoodhash.h

fuzzer: fuzzer.o robinhoodhash.h
	${CXX} $^ -o $@
