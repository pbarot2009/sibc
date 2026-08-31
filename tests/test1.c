// Standard C17 headers and comments
/* Multi-line comment block
   Testing sibc lexer verification */

struct Point {
  int x;
  int y;
};

int compute_total(int *_Nonnull ptr, size_t count) {
  int sum = 0;
  for (size_t i = 0; i < count; ++i) {
    sum += ptr[i] << 1;
  }
  return sum;
}

int main(void) {
  // Numeric literals (decimal, hex, binary, float)
  int dec = 42;
  int hex = 0x2A;
  int bin = 0b101010;
  double pi = 3.14159;

  // String literals and escape sequences
  const char *msg = "Hello\tworld\n";

  // Struct instantiation & arrow operator
  struct Point pt = {.x = 10, .y = 20};
  struct Point *p = &pt;
  int val = p->x + p->y;

  // sibc dialect extensions
  defer free(ptr);

  // Multi-char operators and compound assignment
  int mask = 0xFF;
  mask &= ~(1 << 3);
  mask <<= 2;

  if (dec == 42 && val >= 30) {
    return 0;
  }

  return 1;
}
