#include "checksum_block.h"
#include "crc.h"

#define CRC_SIZE 4

void *block_payload_get(void *block)
{
    return  block + CRC_SIZE;
}

uint32_t block_crc_compute(void *block, size_t size)
{
    void *start = block_payload_get(block);
    size -= CRC_SIZE;

    return crc32(0, start, size);
}

uint32_t block_crc_update(void *block, size_t size)
{
    uint32_t crc;
    uint8_t *myblock = (uint8_t *)block;

    crc = block_crc_compute(block, size);

    myblock[0] = ((crc >> 24) & 0xff);
    myblock[1] = ((crc >> 16) & 0xff);
    myblock[2] = ((crc >> 8) & 0xff);
    myblock[3] = (crc & 0xff);

    return crc;
}

uint32_t block_crc_read(void *block)
{
    uint32_t crc = 0;
    uint8_t *myblock = (uint8_t *)block;

    crc |= myblock[0] << 24;
    crc |= myblock[1] << 16;
    crc |= myblock[2] << 8;
    crc |= myblock[3] << 0;

    return crc;
}

bool block_crc_verify(void *block, size_t size)
{
    uint32_t actual, expected;
    expected = block_crc_read(block);
    actual = block_crc_compute(block, size);
    return expected == actual;
}
