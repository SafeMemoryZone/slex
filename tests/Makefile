.PHONY: test_files, test

fuzz:
	cc -o slex_fuzz -fsanitize=fuzzer,undefined slex_fuzz.c
	./slex_fuzz

test:
	cc -o slex_test slex_test.c
	./slex_test

clean:
	rm slex_test slex_fuzz
