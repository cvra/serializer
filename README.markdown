# Configuration saver

This module is responsible for saving some configuration data, for example in a flash area.
Therefore, it has several responsabilities :

1. Serializing data into a flat buffer
2. Encapsulating those data with a checksum
3. Verifying that the checksum matches the data
4. Unserialize data

It is independent of the storage back end and does not use any dynamic memory allocation.

## Encapsulation with a checksum
This is done by the `checksum_block` module using a CRC32 (IEEE one to be specific) which is then stored before the data.

Sample usage :

```cpp
    /* 64 bytes of data + 4 bytes of CRC. */
    uint8_t raw_block[64 + 4];
    uint8_t *array = block_payload_get(raw_block);

    array[0] = 42;
    /* ... */

    /* Store the CRC in the header. */
    block_crc_update(raw_block, sizeof(raw_block));

    /* Later... */
    if (block_crc_verify(raw_block, sizeof(raw_block))) {
        printf("CRC match, data is probably correct.\n");
    } else {
        printf("CRC mismatch, data is probably corrupt.\n");
    }
```
