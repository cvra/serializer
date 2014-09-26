#include "CppUTest/TestHarness.h"
#include "../serialization.h"
#include "../checksum_block.h"
#include <cstring>


#define BUFFER_SIZE 256


TEST_GROUP(DataSaverIntegrationTesting)
{
    cmp_ctx_t context;
    serializer_t serializer;
    char crc_buffer[BUFFER_SIZE + 4]; // 4 bytes for CRC
    char *buffer;

    void setup()
    {
        // Reset buffer
        memset(crc_buffer, 0, sizeof crc_buffer);
    }
};

TEST(DataSaverIntegrationTesting, SimpleTest)
{
    // Get start of buffer after CRC
    buffer = (char *)block_payload_get(crc_buffer);

    // Create a MessagePack instance in memory
    serializer_init(&serializer, buffer, BUFFER_SIZE);
    serializer_cmp_ctx_factory(&context, &serializer);

    // Write some data
    cmp_write_float(&context, 3.14);
    cmp_write_s32(&context, 42);

    // At this point the checksum is invalid
    CHECK_FALSE(block_crc_verify((void *)crc_buffer, sizeof crc_buffer));

    // Write correct checksum
    block_crc_update(crc_buffer, sizeof crc_buffer);

    // So now the checksum is valid
    CHECK_TRUE(block_crc_verify((void *)crc_buffer, sizeof crc_buffer));

    // We can read back the data.
    // Success will be false if the data is not a float.
    float a;
    bool success;
    success = cmp_read_float(&context, &a);
    CHECK_TRUE(success);
    DOUBLES_EQUAL(3.14, a, 1e-5);

    // Same for integers
    int b;
    success = cmp_read_int(&context, &b);
    CHECK_TRUE(success);
    CHECK_EQUAL(42, b);

    // The CRC is still valid
    CHECK_TRUE(block_crc_verify((void *)crc_buffer, sizeof crc_buffer));

    // But if we poke with data...
    buffer[3] = 0xaa;

    // It should detect it
    CHECK_FALSE(block_crc_verify((void *)crc_buffer, sizeof crc_buffer));
}
