#ifndef SIBC_ARRAY_H
#define SIBC_ARRAY_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct ArrayHeader {
  size_t len;
  size_t cap;
} ArrayHeader;

#define _arr_raw(a) ((ArrayHeader *)(a) - 1)
#define arr_len(a) ((a) ? _arr_raw(a)->len : (size_t)0)
#define arr_cap(a) ((a) ? _arr_raw(a)->cap : (size_t)0)

#define arr_free(a)                                                          \
  do {                                                                       \
    if (a) {                                                                 \
      free(_arr_raw(a));                                                     \
      (a) = NULL;                                                            \
    }                                                                        \
  } while (0)

#define arr_push(a, val)                                                     \
  do {                                                                       \
    _arr_grow((void **)&(a), sizeof(*(a)), arr_len(a) + 1);                  \
    (a)[_arr_raw(a)->len++] = (val);                                         \
  } while (0)

#define arr_pop(a)                                                           \
  do {                                                                       \
    if (arr_len(a) > 0) {                                                    \
      _arr_raw(a)->len--;                                                    \
    }                                                                        \
  } while (0)

static inline void _arr_grow(void **arr_ptr, size_t elem_size, size_t needed) {
  void *a = *arr_ptr;
  size_t cur_cap = arr_cap(a);
  if (needed <= cur_cap)
    return;

  size_t new_cap = (cur_cap == 0) ? 8 : (cur_cap * 2);
  if (new_cap < needed)
    new_cap = needed;

  if (elem_size != 0 && new_cap > (SIZE_MAX - sizeof(ArrayHeader)) / elem_size) {
    fprintf(stderr, "[FATAL] Dynamic array capacity overflow\n");
    exit(EXIT_FAILURE);
  }
  size_t total_size = sizeof(ArrayHeader) + (new_cap * elem_size);

  ArrayHeader *hdr = a ? realloc(_arr_raw(a), total_size) : malloc(total_size);
  if (!hdr) {
    fprintf(stderr, "[FATAL] Dynamic array allocation failed (%zu bytes)\n",
            total_size);
    exit(EXIT_FAILURE);
  }
  if (!a) {
    hdr->len = 0;
  }

  hdr->cap = new_cap;
  *arr_ptr = (void *)(hdr + 1);
}

#endif /* SIBC_ARRAY_H */
