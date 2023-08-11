/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/windows/system/error.hpp"
#include <array>

namespace util
{
    std::string get_error_string(const ::DWORD error_code, const ::WORD lang, const ::WORD sublang)
    {
        constexpr std::size_t buffer_size = 1024;
        std::array<::CHAR, buffer_size> buffer{};

        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, static_cast<DWORD>(MAKELANGID(lang, sublang)),
                        buffer.data(), buffer.size(), nullptr);

        return std::string{buffer.data()};
    }
}
