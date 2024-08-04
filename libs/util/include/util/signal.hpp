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

#include "util/logger.hpp"
#include <csignal>
#include <utility>

#ifndef _WIN32
  #include "util/linux/signal.hpp"
#endif

namespace Util {
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
