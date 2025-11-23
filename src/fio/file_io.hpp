#pragma once

#include <fio/fio.hpp>
#include <memory>
#include <shared_mutex>
#include <unistd.h>

namespace fio {

struct FileDescriptor {
    int fd = -1;

    explicit FileDescriptor(int file_descriptor): fd(file_descriptor) {}

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    FileDescriptor(FileDescriptor&& other) noexcept: fd(other.fd) {
        other.fd = -1;
    }
    FileDescriptor& operator=(FileDescriptor&& other) noexcept {
        if (this != &other) {
            if (fd != -1) {
                ::close(fd);
            }
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    ~FileDescriptor() {
        if (fd != -1) {
            ::close(fd);
        }
    }

    [[nodiscard]] bool is_open() const {
        return this->fd != -1;
    }
};

using errors::Result;
class FileIO: public IOManager {
public:
    using ptr = std::shared_ptr<FileIO>;

    explicit FileIO(int fd);
    FileIO(const FileIO&) = delete;
    FileIO(FileIO&&) = delete;
    FileIO& operator=(const FileIO&) = delete;
    FileIO& operator=(FileIO&&) = delete;

    // IOManager

    Result<uint64_t> read(std::span<uint8_t> buf, uint64_t offset) override;
    Result<uint64_t> write(std::span<const uint8_t> buf) override;
    Result<void> sync() override;
    ~FileIO() override = default;

private:
    FileDescriptor fd;
    std::shared_mutex mtx;
};

} // namespace fio
