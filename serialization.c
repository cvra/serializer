#include <string.h>
#include "serialization.h"


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
