#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_ALIGNMENT 8
#define ALIGN_UP(n, align) (((n) + (align) - 1) & ~((align) - 1))

Arena arena_create(size_t capacity) {
  if (capacity == 0) {
    capacity = ARENA_DEFAULT_CAPACITY;
  }

  uint8_t *buf = (uint8_t *)malloc(capacity);
  if (!buf) {
    fprintf(stderr, "[FATAL] Failed to allocate arena with capacity %zu\n",
            capacity);
    exit(EXIT_FAILURE);
  }

  return (Arena){
      .buffer = buf,
      .capacity = capacity,
      .offset = 0,
  };
}

void *arena_alloc(Arena *arena, size_t size) {
  size_t aligned_offset = ALIGN_UP(arena->offset, ARENA_ALIGNMENT);
  size_t new_offset = aligned_offset + size;

  if (new_offset > arena->capacity) {
    fprintf(stderr,
            "[FATAL] Arena out of memory! Capacity: %zu, Requested: %zu\n",
            arena->capacity, new_offset);
    exit(EXIT_FAILURE);
  }

  void *ptr = arena->buffer + aligned_offset;
  arena->offset = new_offset;
  return ptr;
}

void *arena_alloc_zero(Arena *arena, size_t size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}

void arena_reset(Arena *arena) { arena->offset = 0; }

void arena_destroy(Arena *arena) {
  if (arena->buffer) {
    free(arena->buffer);
    arena->buffer = NULL;
  }
  arena->capacity = 0;
  arena->offset = 0;
}
