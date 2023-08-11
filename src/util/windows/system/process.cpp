/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/windows/system/process.hpp"
#include "util/logger.hpp"
#include "util/windows/system/error.hpp"
#include <array>
#include <cstdlib>
#include <filesystem>

namespace util
{
    std::string get_executable_path()
    {
        std::array<::CHAR, MAX_PATH> buffer{};

        if (0 == ::GetModuleFileName(nullptr, buffer.data(), buffer.size())) {
            return std::string{};
        }

        return canonical(std::filesystem::path{buffer.data()}).string();
    }

    ::HANDLE get_stdin_handle_or_abort()
    {
        auto* const stdin_handle = ::GetStdHandle(STD_INPUT_HANDLE);

        if (INVALID_HANDLE_VALUE == stdin_handle) {
            log_critical("Failed to get standard input handle: {}", get_last_error_string());
            std::abort();
        }

        return stdin_handle;
    }

    ::HANDLE get_stdout_handle_or_abort()
    {
        auto* const stdout_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

        if (INVALID_HANDLE_VALUE == stdout_handle) {
            log_critical("Failed to get standard output handle: {}", get_last_error_string());
            std::abort();
        }

        return stdout_handle;
    }
}
