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
    IO_ERROR,
    FILE_NOT_OPEN,

    // other
};

inline std::string error_to_string(ErrorCode err) {
    static std::map<ErrorCode, std::string> error_map {
        { ErrorCode::IO_ERROR, "I/O operation failed" },
        { ErrorCode::FILE_NOT_OPEN, "File did not open" },
    };
    if (error_map.contains(err)) {
        return error_map[err];
    }
    return "Unknown error";
}

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

private:
    ErrorCode code;
    const std::source_location loc;
    std::string msg;
};

template<typename T>
using Result = std::expected<T, Error>;
} // namespace errors
