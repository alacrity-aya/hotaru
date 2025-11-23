#pragma once

#include <data/log_record.hpp>
#include <format>
#include <source_location>
namespace std {

template<>
struct formatter<std::source_location, char> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const std::source_location& loc, format_context& ctx) {
        return format_to(
            ctx.out(),
            "at {}:{} in {}",
            loc.file_name(),
            loc.line(),
            loc.function_name()
        );
    }
};

template<>
struct formatter<data::LogRecordPos>: std::formatter<const char*> {
private:
    char presentation = ' ';

public:
    constexpr auto parse(std::format_parse_context& ctx) {
        const auto* it = ctx.begin();
        if (it != ctx.end() && (*it == 'd' || *it == 's')) {
            presentation = *it;
            ++it;
        }
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("invalid format for LogRecordPos");
        }
        return it;
    }

    template<typename FormatContext>
    auto format(const data::LogRecordPos& pos, FormatContext& ctx) const {
        if (presentation == 's') {
            return std::format_to(ctx.out(), "({}, {})", pos.file_id, pos.offset);
        }
        return std::format_to(
            ctx.out(),
            "LogRecordPos {{ file_id: {}, offset: {} }}",
            pos.file_id,
            pos.offset
        );
    }
};

} // namespace std
