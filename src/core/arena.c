#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_DEFAULT_ALIGNMENT (alignof(max_align_t))

static size_t align_up(size_t n, size_t alignment) {
  /* alignment must be a power of two. */
  size_t mask = alignment - 1;
  if (n > SIZE_MAX - mask) {
    fprintf(stderr, "[FATAL] Arena alignment overflow\n");
    exit(EXIT_FAILURE);
  }
  return (n + mask) & ~mask;
}

Arena arena_create(size_t capacity) {
  if (capacity == 0) {
    capacity = ARENA_DEFAULT_CAPACITY;
  }

  uint8_t *buf = malloc(capacity);
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

void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment) {
  if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
    fprintf(stderr, "[FATAL] Arena alignment must be a nonzero power of two "
                     "(got %zu)\n",
            alignment);
    exit(EXIT_FAILURE);
  }

  size_t aligned_offset = align_up(arena->offset, alignment);
  if (size > arena->capacity - aligned_offset) {
    fprintf(stderr,
            "[FATAL] Arena out of memory! Capacity: %zu, Used: %zu, "
            "Requested: %zu\n",
            arena->capacity, aligned_offset, size);
    exit(EXIT_FAILURE);
  }

  void *ptr = arena->buffer + aligned_offset;
  arena->offset = aligned_offset + size;
  return ptr;
}

void *arena_alloc(Arena *arena, size_t size) {
  return arena_alloc_aligned(arena, size, ARENA_DEFAULT_ALIGNMENT);
}

void *arena_alloc_zero(Arena *arena, size_t size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}

ArenaMark arena_mark(const Arena *arena) {
  return (ArenaMark){.offset = arena->offset};
}

void arena_reset_to(Arena *arena, ArenaMark mark) {
  if (mark.offset > arena->offset) {
    fprintf(stderr, "[FATAL] Invalid arena mark: %zu exceeds current offset "
                     "%zu\n",
            mark.offset, arena->offset);
    exit(EXIT_FAILURE);
  }
  arena->offset = mark.offset;
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
