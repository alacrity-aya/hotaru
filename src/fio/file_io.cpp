#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fio/file_io.hpp>
#include <format>
#include <mutex>
#include <shared_mutex>
#include <spdlog/spdlog.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.hpp>
namespace fio {

using errors::Error;
using errors::ErrorCode;

FileIO::FileIO(int fd): file_descriptor { fd } {}

Result<FileIO::ptr> FileIO::create(const std::string& filename) {
    int fd = ::open(filename.data(), O_RDWR | O_CREAT | O_CLOEXEC | O_APPEND, 0644);
    if (fd == -1) {
        spdlog::error("open data file err: {}", strerror(errno));
        return std::unexpected { Error {
            ErrorCode::FAILED_TO_OPEN_DATA_FILE,
            std::format("open('{}') failed: {}", filename, strerror(errno)) } };
    }

    return std::make_shared<FileIO>(fd);
}

Result<uint64_t> FileIO::read(std::span<uint8_t> buf, uint64_t offset) {
    std::shared_lock<std::shared_mutex> lock(this->mtx);

    auto bytes_read =
        ::pread(this->file_descriptor.fd, buf.data(), buf.size(), static_cast<int64_t>(offset));
    if (bytes_read == -1) {
        spdlog::error("read from data file err: {}", strerror(errno));
        return std::unexpected {
            Error { ErrorCode::FAILED_TO_READ_FROM_DATA_FILE,
                    std::format("errno: {}", strerror(errno)) },
        };
    }
    return static_cast<uint64_t>(bytes_read);
}
Result<uint64_t> FileIO::write(std::span<const uint8_t> buf) {
    std::unique_lock<std::shared_mutex> lock(this->mtx);
    auto bytes_write = ::write(this->file_descriptor.fd, buf.data(), buf.size());
    if (bytes_write == -1) {
        spdlog::error("write to data file err: {}", strerror(errno));
        return std::unexpected {
            Error { ErrorCode::FAILED_TO_WRITE_TO_DATA_FILE,
                    std::format("errno: {}", strerror(errno)) },
        };
    }
    return static_cast<uint64_t>(bytes_write);
}

Result<void> FileIO::sync() {
    std::shared_lock<std::shared_mutex> lock(this->mtx);
    auto ret = syncfs(this->file_descriptor.fd);

    if (ret == -1) {
        spdlog::error("failed to sync data file: {}", strerror(errno));
        return std::unexpected {
            Error { ErrorCode::FAILED_TO_SYNC_DATA_FILE,
                    std::format("errno: {}", strerror(errno)) },
        };
    }
    return {};
}

} // namespace fio
