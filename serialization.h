#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "cmp.h"

typedef struct {
    char *_block;
    size_t _block_size;
    void *_write_cursor;
    void *_read_cursor;
} serializer_t;

void serializer_init(serializer_t *s, char *block, size_t block_size);

void serializer_write_bytes(serializer_t *s, const char *data, size_t data_size);

void serializer_read_bytes(serializer_t *s, char *data, size_t max_size);

void serializer_cmp_ctx_factory(cmp_ctx_t *context, serializer_t *serializer);

#ifdef __cplusplus
}
#endif


#endif
