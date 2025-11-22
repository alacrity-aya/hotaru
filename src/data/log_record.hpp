#pragma once

#include <cstdint>

namespace data {

struct LogRecordPos {
    uint32_t file_id;
    uint64_t offset;

    bool operator==(const LogRecordPos& other) const {
        return file_id == other.file_id && offset == other.offset;
    }
};

} // namespace data
