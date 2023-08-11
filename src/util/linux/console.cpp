/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/linux/console.hpp"
#include "util/string.hpp"
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>

namespace util
{
    std::size_t get_console_width()
    {
        if (winsize win_size{}; 0 == ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size)) {
            return win_size.ws_col;
        }

        if (const auto* const columns = ::getenv("COLUMNS"); (columns != nullptr) && (*columns != '\0')) {
            return str::convert_to_type<std::size_t>(columns).value_or(0);
        }

        return 0;
    }
}
