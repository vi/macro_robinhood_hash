all: example fuzzer

example: example.o robinhoodhash.h

fuzzer: fuzzer.o robinhoodhash.h
	${CXX} $^ -o $@
