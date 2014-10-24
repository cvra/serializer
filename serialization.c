#include <string.h>
#include "serialization.h"

static bool serializer_cmp_reader(cmp_ctx_t *context, void *data, size_t limit);
static size_t serializer_cmp_writer(cmp_ctx_t *context, const void *data, size_t count);

void serializer_init(serializer_t *s, char *block, size_t block_size)
{
    s->_block = block;
    s->_write_cursor = block;
    s->_read_cursor = block;

    s->_block_size = block_size;
}

void serializer_write_bytes(serializer_t *s, const char *data, size_t data_size)
{
    memcpy(s->_write_cursor, data, data_size);
    s->_write_cursor += data_size;
}

void serializer_read_bytes(serializer_t *s, char *data, size_t max_size)
{
    memcpy(data, s->_read_cursor, max_size);
    s->_read_cursor += max_size;
}

int serializer_written_bytes_count(serializer_t *s)
{
    return (size_t)(s->_write_cursor) - (size_t)(s->_block);
}

void serializer_cmp_ctx_factory(cmp_ctx_t *context, serializer_t *serializer)
{
    cmp_init(context, serializer, serializer_cmp_reader, serializer_cmp_writer);
}

static bool serializer_cmp_reader(cmp_ctx_t *context, void *data, size_t limit)
{
    serializer_t *s;
    s = (serializer_t *)context->buf;
    serializer_read_bytes(s, data, limit);

    return true;
}

static size_t serializer_cmp_writer(cmp_ctx_t *context, const void *data, size_t count)
{
    serializer_t *s;
    s = (serializer_t *)context->buf;
    serializer_write_bytes(s, data, count);
    return count;
}
int serializer_written_bytes_count(serializer_t *s);
