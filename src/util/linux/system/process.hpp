/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <filesystem>
#include <string>
#include <unistd.h>

namespace util
{
    /**
     * @brief Returns the canonical absolute path of the current executable.
     *
     * @return The canonical absolute path of the current executable.
     */
    [[nodiscard]] inline std::string get_executable_path() noexcept
    {
        return canonical(std::filesystem::path{"/proc/self/exe"}).string();
    }

    /**
     * @brief Returns the process ID of the calling process.
     *
     * @return The process ID of the calling process.
     */
    [[nodiscard]] inline int get_process_id() noexcept
    {
        return ::getpid();
    }
}
