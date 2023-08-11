/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <cerrno>
#include <dlfcn.h>
#include <string>

namespace util
{
    /**
     * @brief Returns a string describing the error code.
     *
     * @param error_code The error code to be described.
     *
     * @return A string describing the error code.
     */
    [[nodiscard]] std::string get_error_string(int error_code) noexcept;

    /**
     * @brief Returns a string describing the current value of \c errno
     *
     * @return A string describing the current value of \c errno
     */
    [[nodiscard]] inline std::string get_last_error_string() noexcept
    {
        return get_error_string(errno);
    }

    /**
     * @brief Returns a string describing the last error that occurred during dynamic linking.
     *
     * @return A string describing the last error that occurred, or an empty string if no error occurred.
     */
    [[nodiscard]] inline std::string get_dlerror() noexcept
    {
        const auto* const error = ::dlerror();
        return nullptr == error ? std::string{} : std::string{error};
    }
}
