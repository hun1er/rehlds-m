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

#include <cerrno>
#include <dlfcn.h>
#include <string>

namespace Util {
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
