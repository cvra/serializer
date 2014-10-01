# Configuration saver
[![Build Status](https://travis-ci.org/cvra/serializer.png)](https://travis-ci.org/cvra/serializer)

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

## Serialization
Serializing is done in-memory using the MessagePack format, which is pretty efficient but stills add some metadata to be able to correctly decode the type of the data.

There are two different components at work here :
1. The C MessagePack library
2. An in-memory writer that can be used to write MessagePack objects directly to a memory block.

Example usage (taken from `tests/messagepack_tests.cpp`) :
```cpp
    cmp_ctx_t context;
    serializer_t serializer;
    char buffer[256];
    float b;
    bool success;

    serializer_init(&serializer, buffer, sizeof buffer);
    serializer_cmp_ctx_factory(&context, &serializer);

    cmp_write_float(&context, 3.14);
    success = cmp_read_float(&context, &b);

    if (success) {
        /* b was read correctly. */
    } else {
        printf("Could not read back float !");
    }
```

## Putting it together
The idea is to use a `serializer` which will write into the payload of a CRC'd block.
For an implementation of this idea see `tests/integration_tests.cpp`.
