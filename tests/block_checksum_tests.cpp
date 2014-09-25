#include "CppUTest/TestHarness.h"
#include <cstring>
#include "../checksum_block.h"

TEST_GROUP(ChecksumBlockTestGroup)
{
    uint8_t myblock[64];

    void setup(void)
    {
        memset(myblock, 0, sizeof(myblock));

    }

};

TEST(ChecksumBlockTestGroup, CorrectSpaceIsReservedForChecksum)
{
    // Check that the first four bytes of the block are reserved for the CRC32
    void *payload_start;

    payload_start = block_payload_get((void *)myblock);

    POINTERS_EQUAL(myblock + 4, payload_start);
}

TEST(ChecksumBlockTestGroup, CanComputeBlockChecksum)
{
    uint32_t crc;

    crc = block_crc_compute((void *)myblock, sizeof(myblock));

    CHECK_EQUAL(0x04128908, crc);
}

TEST(ChecksumBlockTestGroup, CanUpdateCheckSum)
{
    uint32_t crc;
    crc = block_crc_update((void *)myblock, sizeof(myblock));
    CHECK_EQUAL(0x08, myblock[0]);
    CHECK_EQUAL(0x89, myblock[1]);
    CHECK_EQUAL(0x12, myblock[2]);
    CHECK_EQUAL(0x04, myblock[3]);
    CHECK_EQUAL(0x04128908, crc);
}

TEST(ChecksumBlockTestGroup, CanGetCheckSumFromHeader)
{
    uint32_t crc;
    myblock[0] = 0x42;
    myblock[1] = 0x43;
    myblock[2] = 0x44;
    myblock[3] = 0x45;

    crc = block_crc_read((void *)myblock);

    POINTERS_EQUAL(0x45444342, crc);
}

TEST(ChecksumBlockTestGroup, NonMatchingCheckSum)
{
    // CRC should be invalid because CRC([0] * 60) != 0
    bool valid;

    valid = block_crc_verify((void *)myblock, sizeof(myblock));

    CHECK_FALSE(valid);
}

TEST(ChecksumBlockTestGroup, testname)
{
    bool valid;

    // Now we update the CRC before checking it so we know it must be valid
    block_crc_update((void *)myblock, sizeof(myblock));
    valid = block_crc_verify((void *)myblock, sizeof(myblock));

    CHECK_TRUE(valid);
}
