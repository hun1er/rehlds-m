/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/console.hpp"
#include "util/logger.hpp"
#include <cstdlib>

namespace util
{
    std::size_t get_console_width()
    {
        ::CONSOLE_SCREEN_BUFFER_INFO buffer_info{};

        if (::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &buffer_info) != FALSE) {
            return buffer_info.srWindow.Right - buffer_info.srWindow.Left + 1;
        }

        return 0;
    }

    ::HWND get_console_window_or_abort()
    {
        auto* const console_window_handle = ::GetConsoleWindow();

        if (nullptr == console_window_handle) {
            log_critical("Failed to get console window handle.");
            std::abort();
        }

        return console_window_handle;
    }
}
