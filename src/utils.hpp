#pragma once

#include <cpptrace/cpptrace.hpp>
#include <cstdlib>
#include <print>
#include <source_location>
#include <string_view>

namespace utils {

[[noreturn]] inline void
panic(std::string_view msg, const std::source_location& loc = std::source_location::current()) {
    std::println(
        "\033[31mPANIC at {}:{} in {}: {}\033[0m",
        loc.file_name(),
        loc.line(),
        loc.function_name(),
        msg
    );

    cpptrace::generate_trace().print();
    std::abort();
}

[[noreturn]] inline void todo(const std::source_location& loc = std::source_location::current()) {
    panic("Not implemented", loc);
}

} // namespace utils
