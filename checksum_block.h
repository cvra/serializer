#ifndef CHECKSUM_BLOCK
#define CHECKSUM_BLOCK

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void *block_payload_get(void *block);
uint32_t block_crc_compute(void *block, size_t size);
uint32_t block_crc_update(void *block, size_t size);
uint32_t block_crc_read(void *block);
bool block_crc_verify(void *block, size_t size);

#ifdef __cplusplus
}
#endif

#endif
