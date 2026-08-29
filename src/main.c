#include <assert.h>
#include <stdio.h>

#include "core/arena.c"
#include "core/arena.h"
#include "core/array.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  printf("sibc: Initializing runtime primitives...\n");

  // 1. Test Arena Allocator
  Arena ast_arena = arena_create(1024 * 1024);
  int *numbers = (int *)arena_alloc(&ast_arena, sizeof(int) * 3);
  numbers[0] = 10;
  numbers[1] = 20;
  numbers[2] = 30;

  assert(numbers[0] + numbers[1] + numbers[2] == 60);
  printf("[PASS] Arena allocation (used: %zu bytes)\n", ast_arena.offset);

  // 2. Test Dynamic Array (Stretchy Buffer)
  int *token_stream = NULL;
  for (int i = 1; i <= 100; i++) {
    arr_push(token_stream, i * 2);
  }

  assert(arr_len(token_stream) == 100);
  assert(token_stream[0] == 2);
  assert(token_stream[99] == 200);
  printf("[PASS] Dynamic array (count: %zu, cap: %zu)\n", arr_len(token_stream),
         arr_cap(token_stream));

  // Cleanup
  arr_free(token_stream);
  arena_destroy(&ast_arena);

  printf("All core tests passed successfully!\n");
  return 0;
}
