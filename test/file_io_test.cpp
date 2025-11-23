#include <filesystem>
#include <fio/file_io.hpp>
#include <fio/fio.hpp>
#include <gtest/gtest.h>

using namespace std; //NOLINT
using namespace fio; //NOLINT
using namespace errors; //NOLINT

static const string TEST_FILE = "test_data.dat";

class FileIOTest: public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove(TEST_FILE);
    }
};

TEST_F(FileIOTest, CreateSuccess) {
    auto res = FileIO::create(TEST_FILE);
    ASSERT_TRUE(res.has_value());
}

TEST_F(FileIOTest, CreateFail) {
    std::filesystem::create_directory("dir_test");

    auto res = FileIO::create("dir_test");
    ASSERT_FALSE(res.has_value());
    EXPECT_EQ(res.error().get_code(), ErrorCode::FAILED_TO_OPEN_DATA_FILE);

    std::filesystem::remove("dir_test");
}

TEST_F(FileIOTest, WriteAndRead) {
    auto fio_res = FileIO::create(TEST_FILE);
    ASSERT_TRUE(fio_res.has_value());
    const auto& fio = fio_res.value();

    std::vector<uint8_t> write_data = { 1, 2, 3, 4, 5, 6, 7, 8 };
    auto wr = fio->write(write_data);
    ASSERT_TRUE(wr.has_value());
    EXPECT_EQ(wr.value(), write_data.size());

    std::vector<uint8_t> read_buf(8);
    auto rr = fio->read(read_buf, 0);
    ASSERT_TRUE(rr.has_value());
    EXPECT_EQ(rr.value(), write_data.size());

    EXPECT_EQ(read_buf, write_data);
}

TEST_F(FileIOTest, ReadWithOffset) {
    auto fio_res = FileIO::create(TEST_FILE);
    ASSERT_TRUE(fio_res.has_value());
    const auto& fio = fio_res.value();

    // write 16 bytes
    std::vector<uint8_t> write_data(16);
    for (int i = 0; i < 16; i++)
        write_data[i] = i;

    auto wr = fio->write(write_data);
    ASSERT_TRUE(wr.has_value());

    // offset 8
    std::vector<uint8_t> buf(4);
    auto rr = fio->read(buf, 8);
    ASSERT_TRUE(rr.has_value());
    EXPECT_EQ(rr.value(), 4);

    EXPECT_EQ(buf[0], 8);
    EXPECT_EQ(buf[1], 9);
    EXPECT_EQ(buf[2], 10);
    EXPECT_EQ(buf[3], 11);
}

TEST_F(FileIOTest, SyncTest) {
    auto fio_res = FileIO::create(TEST_FILE);
    ASSERT_TRUE(fio_res.has_value());
    const auto& fio = fio_res.value();

    auto res = fio->sync();
    ASSERT_TRUE(res.has_value());
}

TEST_F(FileIOTest, MultiThreadReadAndWrite) {
    auto fio_res = FileIO::create(TEST_FILE);
    ASSERT_TRUE(fio_res.has_value());
    auto fio = fio_res.value();
    std::vector<uint8_t> data(1024, 42);
    std::thread writer([&]() {
        for (int i = 0; i < 100; i++)
            ASSERT_TRUE(fio->write(data).has_value());
    });

    std::thread reader([&]() {
        std::vector<uint8_t> buf(1024);
        for (int i = 0; i < 100; i++)
            ASSERT_TRUE(fio->read(buf, 0).has_value());
    });

    writer.join();
    reader.join();

    SUCCEED();
}
