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

/** Initializes a writer to the given memory block. */
void serializer_init(serializer_t *s, char *block, size_t block_size);

/** Writes data_size bytes from data to the current position in the writer. */
void serializer_write_bytes(serializer_t *s, const char *data, size_t data_size);

/** Reads max_size bytes from the reading position to data. */
void serializer_read_bytes(serializer_t *s, char *data, size_t max_size);

/** Returns the amount of bytes written on this serializer. */
int serializer_written_bytes_count(serializer_t *s);

/** Creates a MessagePack context using the given serializer. */
void serializer_cmp_ctx_factory(cmp_ctx_t *context, serializer_t *serializer);

#ifdef __cplusplus
}
#endif


#endif
