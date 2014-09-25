/* Licensed under GPLv2+ - see LICENSE file for details */
#ifndef CCAN_CRC_H
#define CCAN_CRC_H

#ifdef __cpluscplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/**
 * crc32_ieee - IEEE 802.3 32 bit crc of string of bytes
 * @start_crc: the initial crc (usually 0)
 * @buf: pointer to bytes
 * @size: length of buffer
 *
 * 32 bit CRC checksum using polynomial
 * X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0.
 *
 * See crc32c() for details.
 */
uint32_t crc32(uint32_t start_crc, const void *buf, size_t size);
#ifdef __cpluscplus
}
#endif
#endif /* CCAN_CRC_H */
