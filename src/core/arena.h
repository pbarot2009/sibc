#ifndef SIBC_ARENA_H
#define SIBC_ARENA_H

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ARENA_DEFAULT_CAPACITY (4 * 1024 * 1024) /* 4 MB */

typedef struct Arena {
  uint8_t *buffer;
  size_t capacity;
  size_t offset;
} Arena;

/* Opaque save point for arena_reset_to(). Obtain with arena_mark(). */
typedef struct ArenaMark {
  size_t offset;
} ArenaMark;

Arena arena_create(size_t capacity);
void *arena_alloc(Arena *arena, size_t size);
void *arena_alloc_zero(Arena *arena, size_t size);
void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment);

ArenaMark arena_mark(const Arena *arena);
void arena_reset_to(Arena *arena, ArenaMark mark);
void arena_reset(Arena *arena);
void arena_destroy(Arena *arena);

#endif /* SIBC_ARENA_H */
