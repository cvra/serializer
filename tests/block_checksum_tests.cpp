#include "CppUTest/TestHarness.h"
#include <cstring>
#include "../checksum_block.h"

TEST_GROUP(ChecksumBlockTestGroup)
{
    uint8_t myblock[64];
    uint32_t crc;
    bool valid;

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
    // Checks if we can compute the CRC of the block
    crc = block_crc_compute((void *)myblock, sizeof(myblock));

    CHECK_EQUAL(0x04128908, crc);
}

TEST(ChecksumBlockTestGroup, CanUpdateCheckSum)
{
    // Checks if the correct CRC gets written to the header
    crc = block_crc_update((void *)myblock, sizeof(myblock));

    CHECK_EQUAL(0x04, myblock[0]);
    CHECK_EQUAL(0x12, myblock[1]);
    CHECK_EQUAL(0x89, myblock[2]);
    CHECK_EQUAL(0x08, myblock[3]);
    CHECK_EQUAL(0x04128908, crc);
}

TEST(ChecksumBlockTestGroup, CanGetCheckSumFromHeader)
{
    // Checks if we can read the CRC back from the header
    myblock[0] = 0x45;
    myblock[1] = 0x44;
    myblock[2] = 0x43;
    myblock[3] = 0x42;

    crc = block_crc_read((void *)myblock);

    CHECK_EQUAL(0x45444342, crc);
}

TEST(ChecksumBlockTestGroup, NonMatchingCheckSum)
{
    // CRC should be invalid because CRC([0] * 60) != 0
    valid = block_crc_verify((void *)myblock, sizeof(myblock));

    CHECK_FALSE(valid);
}

TEST(ChecksumBlockTestGroup, testname)
{
    // Now we update the CRC before checking it so we know it must be valid
    block_crc_update((void *)myblock, sizeof(myblock));
    valid = block_crc_verify((void *)myblock, sizeof(myblock));

    CHECK_TRUE(valid);
}
