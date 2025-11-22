#pragma once

#include <cstdint>
#include <data/log_record.hpp>
#include <optional>
#include <vector>

namespace indexer {

using data::LogRecordPos;

class Indexer {
public:
    virtual bool put(std::vector<uint8_t> key, LogRecordPos pos) = 0;
    virtual std::optional<LogRecordPos> get(std::vector<uint8_t>) = 0;
    virtual bool del(std::vector<uint8_t> key) = 0;
    virtual ~Indexer() = default;
};

} // namespace indexer
