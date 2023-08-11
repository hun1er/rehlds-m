/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <functional>

namespace util
{
    /**
     * @brief Registers a callback function to be called when the program exits normally
     * or when \c std::quick_exit is called.
     *
     * The callback functions are stored in a stack data structure (LIFO)
     * and are called in reverse order of registration.
     *
     * @param callback The callback function to register.
     *
     * @return \c true if the callback was successfully registered, \c false otherwise.
     */
    bool at_exit(std::function<void()> callback) noexcept;
}
