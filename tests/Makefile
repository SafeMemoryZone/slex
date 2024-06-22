.PHONY: clean all

all: test fuzz

fuzz:
	cc -o fuzz -fsanitize=fuzzer,undefined slex_fuzz.c
	./fuzz

test:
	cc -o test slex_test.c
	./test

clean:
	rm test fuzz
