/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <cstddef>
#include <Windows.h>

namespace util
{
    /**
     * @brief Retrieves the current width of the console.
     *
     * @return The current width of the console, or 0 if the width could not be determined.
     */
    [[nodiscard]] std::size_t get_console_width();

    /**
     * @brief Retrieves the console window handle or aborts the program.
     *
     * If the console window handle cannot be retrieved, the program is aborted.
     *
     * @return The console window handle.
     */
    [[nodiscard]] ::HWND get_console_window_or_abort();
}
