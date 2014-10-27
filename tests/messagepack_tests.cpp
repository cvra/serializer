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

TEST(MessagePackTestGroup, CanWriteString)
{
    char data[30];
    uint32_t data_len;
    bool res;
    char expected[] = "Hello";

    cmp_write_str(&context, expected, 5);

    // The data length parameter is used for both maximal size (in) and real size (out)
    data_len = sizeof(data);

    res = cmp_read_str(&context, data, &data_len);
    CHECK_TRUE(res);
    STRCMP_EQUAL(expected, data);
}

TEST(MessagePackTestGroup, CanWriteCustomBinaryType)
{
    char data[] = {42, 43, 44};

    char data2[3];
    int8_t type = 12;
    uint32_t size = 3;
    bool success;

    cmp_write_ext(&context, type, size, data);

    // Ext 8 header
    BYTES_EQUAL(0xc7, buffer[0]);

    // Data length
    BYTES_EQUAL(3, buffer[1]);

    // Data type
    BYTES_EQUAL(12, buffer[2]);

    for (int i = 0; i < 3; ++i) {
        BYTES_EQUAL(data[i], buffer[3+i]);
    }

    success = cmp_read_ext(&context, &type, &size, data2);
    CHECK_TRUE(success);
    CHECK_EQUAL(12, type);
    CHECK_EQUAL(3, size);

    for (int i = 0; i < 3; ++i) {
        BYTES_EQUAL(data[i], data2[i]);
    }
}
