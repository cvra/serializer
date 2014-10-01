#ifndef CHECKSUM_BLOCK
#define CHECKSUM_BLOCK

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/** Returns a pointer to the first usable byte after the block header. */
void *block_payload_get(void *block);

/** Returns the CRC of a block. */
uint32_t block_crc_compute(void *block, size_t size);

/** Writes the CRC to the block header.
 * @returns Block CRC that was written to the header.
 */
uint32_t block_crc_update(void *block, size_t size);

/** Reads the CRC from the header. */
uint32_t block_crc_read(void *block);

/** Checks if the CRC of the block matches the one stored in the header. */
bool block_crc_verify(void *block, size_t size);

#ifdef __cplusplus
}
#endif

#endif
