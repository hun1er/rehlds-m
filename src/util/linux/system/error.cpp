/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/linux/system/error.hpp"
#include <array>
#include <cstring>

namespace util
{
    std::string get_error_string(const int error_code) noexcept
    {
        constexpr std::size_t buffer_size = 1024;

        std::array<char, buffer_size> buffer{};
        const auto* const message = ::strerror_r(error_code, buffer.data(), buffer.size());

        return nullptr == message ? std::string{} : std::string{message};
    }
}
