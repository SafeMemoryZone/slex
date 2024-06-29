// This is a sample file used for testing
#include <stdio.h>
#include <stdlib.h>
#define SOME_MACRO 1 \
  a \
  b

    #define SOME_OTHER_MACRO 2

int foo(int a, int b, ...) {
  return a + b;
}

/*
 * This is a block comment
 */
typedef struct {
  int a;
} SomeStruct;

void some_function() {
  if(1 == 1) {
    printf("Hello, World\n");
  }
    // should equal to "HELLO"
  char *str = "\x48\105\114\x4C\x4F\0";
  SomeStruct* s = malloc(sizeof(SomeStruct));
  
  int hex = 0xFFFFFF3;
  int oct = 011;
  long num = 12;

  *str = 12;
  int yes = s->a == 12 || 2 == 1;
  if(*str >= yes && yes == 1 || (yes += 1)) {
    yes ~= 2;
    yes++;
  }
                                    int indent = 1; // to much indentation
  some_function();

  // now the errors:
  int too_big = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
  too_big = 07777777777777777777777777777777ll; // suffix
  too_big = 1239812093812093812098312098321098321;
  unsigned long long max_number = 18446744073709551615;
  hello::world;
}
