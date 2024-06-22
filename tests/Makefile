.PHONY: clean all fuzz test

all: test fuzz

fuzz:
	@cc -o build/fuzz -fsanitize=fuzzer,address,undefined -g slex_fuzz.c
	@./build/fuzz

test:
	@cc -o build/test -fsanitize=address,undefined -g slex_test.c
	@./build/test
