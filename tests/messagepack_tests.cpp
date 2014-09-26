#include "CppUTest/TestHarness.h"
#include "../serialization.h"
#include <cstring>

TEST_GROUP(MessagePackTestGroup)
{
    cmp_ctx_t context;
    serializer_t serializer;
    char buffer[256];

    void setup()
    {
        memset(buffer, 0, sizeof buffer);
        serializer_init(&serializer, buffer, sizeof buffer);
        serializer_cmp_ctx_factory(&context, &serializer);
    }
};

TEST(MessagePackTestGroup, CheckMessagePackVersion)
{
    // Forces the use of MessagePack 5 to avoid updating to non compatible
    // version
    uint32_t version = cmp_mp_version();

    CHECK_EQUAL(5, version);
}

TEST(MessagePackTestGroup, CanWriteFloat)
{
    float a = 3.14;
    float b;
    bool success;

    cmp_write_float(&context, a);
    success = cmp_read_float(&context, &b);

    BYTES_EQUAL(0xca, buffer[0]); // float header
    CHECK_TRUE(success);
    CHECK_EQUAL(a, b);
}
