/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <string>
#include <Windows.h>

namespace util
{
    /**
     * @brief Returns a string representation of the error message associated with the given error code.
     *
     * @param error_code The error code to retrieve the message for.
     * @param lang The language identifier for the message.
     * @param sublang The sublanguage identifier for the message.
     *
     * @return The error message associated with the given error code.
     */
    [[nodiscard]] std::string get_error_string(::DWORD error_code, ::WORD lang = LANG_ENGLISH,
                                               ::WORD sublang = SUBLANG_ENGLISH_US);

    /**
     * @brief Returns a string representation of the error message associated with the last error that occurred.
     *
     * @param lang The language identifier for the message.
     * @param sublang The sublanguage identifier for the message.
     *
     * @return The error message associated with the last error that occurred.
     */
    [[nodiscard]] inline std::string get_last_error_string(const ::WORD lang = LANG_ENGLISH,
                                                           const ::WORD sublang = SUBLANG_ENGLISH_US)
    {
        return get_error_string(::GetLastError(), lang, sublang);
    }
}
