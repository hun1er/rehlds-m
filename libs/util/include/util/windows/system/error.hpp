/*
 * Half Life 1 SDK License
 * Copyright(c) Valve Corp
 *
 * DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 * DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 * DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 * FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 * LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 * SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * For commercial use, contact: sourceengine@valvesoftware.com
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <string>
#include <Windows.h>

namespace Util {
    /**
     * @brief Returns a string representation of the error message associated with the given error code.
     *
     * @param error_code The error code to retrieve the message for.
     * @param lang The language identifier for the message.
     * @param sublang The sublanguage identifier for the message.
     *
     * @return The error message associated with the given error code.
     */
    [[nodiscard]] std::string
      get_error_string(::DWORD error_code, ::WORD lang = LANG_ENGLISH, ::WORD sublang = SUBLANG_ENGLISH_US);

    /**
     * @brief Returns a string representation of the error message associated with the last error that occurred.
     *
     * @param lang The language identifier for the message.
     * @param sublang The sublanguage identifier for the message.
     *
     * @return The error message associated with the last error that occurred.
     */
    [[nodiscard]] inline std::string
      get_last_error_string(const ::WORD lang = LANG_ENGLISH, const ::WORD sublang = SUBLANG_ENGLISH_US)
    {
        return get_error_string(::GetLastError(), lang, sublang);
    }
}
