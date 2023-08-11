/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "util/logger.hpp"
#include <csignal>
#include <utility>

#ifndef _WIN32
  #include "util/linux/signal.hpp"
#endif

namespace util
{
    /**
     * @brief Attempt to set a signal handler for the specified signal and log an error message on failure.
     *
     * @tparam Handler The type of the signal handler.
     *
     * @param signal The signal to set the handler for.
     * @param handler The signal handler to set.
     *
     * @return \c true if the signal handler was successfully set, \c false otherwise.
     */
    template <typename Handler>
    bool try_set_signal_handler(const int signal, Handler&& handler) noexcept
    {
        if (SIG_ERR == std::signal(signal, std::forward<Handler>(handler))) {
            log_error("Failed to set signal handler for signal {}.", signal);
            return false;
        }

        return true;
    }
}
