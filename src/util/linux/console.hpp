/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <cstddef>

namespace util
{
    /**
     * @brief Retrieves the current width of the console.
     *
     * @return The current width of the console, or 0 if the width could not be determined.
     */
    std::size_t get_console_width();
}
