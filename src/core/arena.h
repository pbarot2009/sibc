#ifndef SIBC_ARENA_H
#define SIBC_ARENA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ARENA_DEFAULT_CAPACITY (4 * 1024 * 1024) // 4 MB

typedef struct Arena {
  uint8_t *buffer;
  size_t capacity;
  size_t offset;
} Arena;

Arena arena_create(size_t capacity);
void *arena_alloc(Arena *arena, size_t size);
void *arena_alloc_zero(Arena *arena, size_t size);
void arena_reset(Arena *arena);
void arena_destroy(Arena *arena);

#endif // SIBC_ARENA_H
