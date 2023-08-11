/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <process.h>
#include <string>
#include <Windows.h>

namespace util
{
    /**
     * @brief Returns the canonical absolute path of the current executable.
     *
     * @return The canonical absolute path of the current executable.
     */
    [[nodiscard]] std::string get_executable_path();

    /**
     * @brief Returns the process ID of the calling process.
     *
     * @return The process ID of the calling process.
     */
    [[nodiscard]] inline int get_process_id()
    {
        return ::_getpid();
    }

    /**
     * @brief Retrieves the standard input handle or aborts the program.
     *
     * If the standard input handle cannot be retrieved, the program is aborted.
     *
     * @return The standard input handle.
     */
    [[nodiscard]] ::HANDLE get_stdin_handle_or_abort();

    /**
     * @brief Retrieves the standard output handle or aborts the program.
     *
     * If the standard output handle cannot be retrieved, the program is aborted.
     *
     * @return The standard output handle.
     */
    [[nodiscard]] ::HANDLE get_stdout_handle_or_abort();
}
