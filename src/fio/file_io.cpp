#include <array>
#include <cerrno>
#include <cstring>
#include <fio/file_io.hpp>
#include <format>
#include <print>
#include <shared_mutex>
#include <sys/types.h>
#include <unistd.h>
#include <utils.hpp>
namespace fio {

using errors::Error;
using errors::ErrorCode;

FileIO::FileIO(int fd): fd { fd } {}

Result<uint64_t> FileIO::read(std::span<uint8_t> buf, uint64_t offset) {
    std::shared_lock<std::shared_mutex> lock(this->mtx);

    if (this->fd.is_open()) {
        return std::unexpected { Error { ErrorCode::FILE_NOT_OPEN } };
    }

    auto bytes_read = ::pread(this->fd.fd, buf.data(), buf.size(), static_cast<int64_t>(offset));
    if (bytes_read == -1) {
        return std::unexpected {
            Error { ErrorCode::IO_ERROR, std::format("errno: {}", strerror(errno)) },
        };
    }
    return static_cast<uint64_t>(bytes_read);
}
Result<uint64_t> FileIO::write(std::span<const uint8_t> buf) {
    utils::todo();
}

Result<void> FileIO::sync() {
    utils::todo();
}

} // namespace fio
