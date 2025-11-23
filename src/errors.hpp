#pragma once

#include <cstdint>
#include <expected>
#include <format>
#include <map>
#include <printable.hpp>
#include <source_location>
#include <string>
#include <utility>

namespace errors {

enum class ErrorCode : uint8_t {

    // File operation
    FAILED_TO_READ_FROM_DATA_FILE,
    FAILED_TO_WRITE_TO_DATA_FILE,
    FAILED_TO_SYNC_DATA_FILE,
    FAILED_TO_OPEN_DATA_FILE,

    // other
};

inline std::string error_to_string(ErrorCode err) {
    static std::map<ErrorCode, std::string> error_map {
        { ErrorCode::FAILED_TO_READ_FROM_DATA_FILE, "failed to read from data file" },
        { ErrorCode::FAILED_TO_WRITE_TO_DATA_FILE, "failed to write to data file" },
        { ErrorCode::FAILED_TO_SYNC_DATA_FILE, "failed to sync data file" },
        { ErrorCode::FAILED_TO_OPEN_DATA_FILE, "failed to open data file" },
    };
    if (error_map.contains(err)) {
        return error_map[err];
    }
    return "Unknown error";
}

struct Error;

template<typename T>
using Result = std::expected<T, Error>;

struct Error {
public:
    explicit Error(
        ErrorCode code,
        std::string msg = "",
        std::source_location loc = std::source_location::current()
    ):
        code { code },
        loc { loc },
        msg { std::move(msg) } {}

    [[nodiscard]] std::string to_string() const {
        auto base_msg = error_to_string(code);
        return std::format("{}: {} {}", loc, base_msg, msg);
    }

    ErrorCode get_code() {
        return this->code;
    }

private:
    ErrorCode code;
    const std::source_location loc;
    std::string msg;
};

} // namespace errors
