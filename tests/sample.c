#define MACRO1 1
#define MACRO2(x) (x + 1)
#define MACRO3(x, y) ((x) * (y))
#define MACRO4 "Macro with backslash: \\"
#define MACRO5 "Macro with double quote: \""
#define MACRO6 "Macro with newline: \n"
#define LONG_MACRO 1 \
  2 \
  3\

int some_other_funcion(int a, int b, ...) {
  return a + b;
}

int main() {
    // String/Character literals
    char *str1 = "hello, world\n";
    char *str2 = "abc\0def";
    char *str3 = "octal: \101";      // A
    char *str4 = "hex: \x41";        // A
    char *str5 = "unicode: \u03bb";  // some greek letter
    char *str6 = "backslash: \\";
    char *str7 = "double quote: \"";
    char *str8 = "single quote: \'";

    // Integer literals
    int int1 = 0x12;
    int int2 = 123;
    int int3 = 030;
    int int4 = 0b111;

    // Floats
    double mypi = 3.14;
    mypi = 34.123213093871;
    double tau = 6.; // rounded
    double some1 = 32.1e2;
    double some2 = 32.1e+2;
    double some3 = 32.1e-2;

    // Identifiers
    int foo = 1e3;
    int bar = 2e2;

    // Punctuators and Operators
int array[10];                
    array[0] = foo;               
int x = (foo + bar);          
    {                             
        int y = foo * bar;        
    }

    int a = foo & bar;
    int b = foo && bar;
    a &= b;
    int c = foo * bar;
    c *= 2;
    int d = foo + bar;
    d++;
    d += 1;
    int e = foo - bar;
    e--;
    e -= 1;
    int f = ~foo;
    int g = !foo;
    int h = foo != bar;
    int i = foo / bar;
    i /= 2;
    int j = foo % bar;
    j %= 2;
    int k = foo < bar;
    int l = foo << 1;
    k <= l;
    l <<= 1;
    int m = foo > bar;
    int n = foo >> 1;
    m >= n;
    n >>= 1;
    int o = foo ^ bar;
    o ^= bar;
    int p = foo | bar;
    int q = foo || bar;
    p |= bar;
    int r = foo ? bar : bar;
    int s = foo == bar;
    int t = foo = bar;

    long long v = 0xFFFFFFFFFFF;
    int oct = 051;
    int hex = 0x51;
    int bin = 0b11111111;

    // Comments
    // Single-line comment

    /*
     * Multi-line comment
     */

    // Macro usage
    int macroTest1 = MACRO1;
    int macroTest2 = MACRO2(5);
    int macroTest3 = MACRO3(2, 3);
    char *macroTest4 = MACRO4;
    char *macroTest5 = MACRO5;
    char *macroTest6 = MACRO6;

    int edgeCase1 = 0; /* Inline comment */ int edgeCase2 = 1;

    /* Comment starts here
       and ends here */
    int edgeCase3 = 2;

    // Test the backslash as an escape character
    char backslash = '\\';

    return 0;
}

