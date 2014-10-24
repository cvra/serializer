#include "CppUTest/TestHarness.h"
#include <cstring>
#include "../serialization.h"

extern "C" {
#include "cmp.h"
}

TEST_GROUP(SerializerTestGroup)
{
    serializer_t s;
    char myblock[30];
    void setup() {
        serializer_init(&s, myblock, sizeof myblock);
        memset(myblock, 0, sizeof myblock);
    }
};

TEST(SerializerTestGroup, CanCreateSerializer)
{
    POINTERS_EQUAL(myblock, s._block);
    CHECK_EQUAL(sizeof myblock, s._block_size);
    POINTERS_EQUAL(myblock, s._write_cursor);
    POINTERS_EQUAL(myblock, s._read_cursor);
}

TEST(SerializerTestGroup, CanWriteABuffer)
{
    const char *data = "hello";
    serializer_write_bytes(&s, data, 5);
    STRCMP_EQUAL(data, myblock);
}

TEST(SerializerTestGroup, WriteMovesWriteHead)
{
    const char *data = "hello";
    serializer_write_bytes(&s, data, 5);
    POINTERS_EQUAL(s._write_cursor, &myblock[5]);
}

TEST(SerializerTestGroup, CanWriteMultipleTimes)
{
    const char *data1 = "hel";
    const char *data2 = "lo";
    serializer_write_bytes(&s, data1, 3);
    serializer_write_bytes(&s, data2, 2);
    STRCMP_EQUAL("hello", myblock);
}

TEST(SerializerTestGroup, CanRead)
{
    char data[5 + 1];
    data[5] = 0;

    serializer_write_bytes(&s, "hello", 5);
    serializer_read_bytes(&s, data, 5);

    STRCMP_EQUAL("hello", data);
}

TEST(SerializerTestGroup, ReadMovesReadHead)
{
    char data[5];
    serializer_read_bytes(&s, data, 5);
    POINTERS_EQUAL(&myblock[5], s._read_cursor);
}

TEST(SerializerTestGroup, CanReadManyTimes)
{
    char data1[3 + 1];
    char data2[2 + 1];
    data1[3] = data2[2] = 0;

    serializer_write_bytes(&s, "hello", 5);
    serializer_read_bytes(&s, data1, 3);
    serializer_read_bytes(&s, data2, 2);

    STRCMP_EQUAL("hel", data1);
    STRCMP_EQUAL("lo", data2);
}

TEST(SerializerTestGroup, WrittenCountIsZeroInitially)
{
    CHECK_EQUAL(0, serializer_written_bytes_count(&s));
}

TEST(SerializerTestGroup, WrittenCountIsChanged)
{
    serializer_write_bytes(&s, "hello", 5);
    CHECK_EQUAL(5, serializer_written_bytes_count(&s));
}

TEST_GROUP(CMPApiTestGroup)
{
    cmp_ctx_t ctx;
    char buffer[64];
    char data[64];
    serializer_t s;

    void setup()
    {
        serializer_init(&s, buffer, sizeof buffer);
        serializer_cmp_ctx_factory(&ctx, &s);
        memset(buffer, 0, sizeof buffer);
        memset(data, 0, sizeof data);
    }
};

TEST(CMPApiTestGroup, CanInitContext)
{
    POINTERS_EQUAL(&s, ctx.buf);
}

TEST(CMPApiTestGroup, ReaderWorksAsExpected)
{
    bool read_successful;

    serializer_write_bytes(&s, "hello", 5);

    read_successful = ctx.read(&ctx, data, sizeof data);

    STRCMP_EQUAL("hello", data);
    CHECK_TRUE(read_successful);
}

TEST(CMPApiTestGroup, WriterWorksAsExpected)
{
    size_t l;
    l = ctx.write(&ctx, "xkcd", 5);
    serializer_read_bytes(&s, data, sizeof data);

    STRCMP_EQUAL("xkcd", data);
    CHECK_EQUAL(5, l);
}
