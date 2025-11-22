#include <gtest/gtest.h>
#include <index/btree.hpp>

TEST(BTreeFunctionalTest, BasicOperations) {
    indexer::BTree btree;
    std::vector<uint8_t> key1 { 1, 1, 1 };
    std::vector<uint8_t> key2 { 2, 2, 2 };
    data::LogRecordPos pos1 = { .file_id = 1, .offset = 100 };
    data::LogRecordPos pos2 = { .file_id = 2, .offset = 200 };

    EXPECT_FALSE(btree.get(key1).has_value());

    EXPECT_TRUE(btree.put(key1, pos1));

    auto result1 = btree.get(key1);
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value(), pos1);

    data::LogRecordPos pos1_new = { .file_id = 1, .offset = 101 };
    EXPECT_TRUE(btree.put(key1, pos1_new));
    auto result1_updated = btree.get(key1);
    EXPECT_TRUE(result1_updated.has_value());
    EXPECT_EQ(result1_updated.value(), pos1_new);

    EXPECT_TRUE(btree.del(key1));

    EXPECT_FALSE(btree.del(key1));

    EXPECT_FALSE(btree.get(key1).has_value());

    EXPECT_TRUE(btree.put(key2, pos2));
    EXPECT_TRUE(btree.get(key2).has_value());
    EXPECT_EQ(btree.get(key2).value(), pos2);
}
