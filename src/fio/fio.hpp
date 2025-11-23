#pragma once

#include <cstdint>
#include <errors.hpp>
#include <span>
namespace fio {

using errors::Result;

class IOManager {
public:
    virtual Result<uint64_t> read(std::span<uint8_t> buf, uint64_t offset) = 0;
    virtual Result<uint64_t> write(std::span<const uint8_t> buf) = 0;
    virtual Result<void> sync() = 0;
    virtual ~IOManager() = default;
};

} // namespace fio
