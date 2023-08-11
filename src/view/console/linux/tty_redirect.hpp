/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "util/logger.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <unistd.h>

namespace view
{
    /**
     * @brief A RAII class for redirecting TTY output to \c std::cout within a scope.
     */
    class ScopedTtyRedirect final
    {
      public:
        /**
         * @brief Construct a new ScopedTtyRedirect object and redirect TTY output to \c std::cout
         */
        ScopedTtyRedirect()
        {
            const auto* term_name = ::ctermid(nullptr); // cppcheck-suppress ctermidCalled

            if ((nullptr == term_name) || ('\0' == *term_name) || (0 == ::isatty(STDIN_FILENO))) {
                term_name = "/dev/null";
            }

            // Disable buffering for TTY
            tty_.rdbuf()->pubsetbuf(nullptr, 0);

            // Open TTY for output
            tty_.open(term_name, std::ios_base::out);

            if (tty_.is_open()) {
                // Set TTY as buffer for std::cout
                original_buffer_ = std::cout.rdbuf(tty_.rdbuf());
            }
            else {
                util::log_error("Failed to open TTY '{}' for output.", term_name);
            }
        }

        /// Move constructor.
        ScopedTtyRedirect(ScopedTtyRedirect&&) = delete;

        /// Copy constructor.
        ScopedTtyRedirect(const ScopedTtyRedirect&) = delete;

        /// Move assignment operator.
        ScopedTtyRedirect& operator=(ScopedTtyRedirect&&) = delete;

        /// Copy assignment operator.
        ScopedTtyRedirect& operator=(const ScopedTtyRedirect&) = delete;

        /**
         * @brief Destroy the ScopedTtyRedirect object and restore the original buffer for \c std::cout
         */
        ~ScopedTtyRedirect()
        {
            // Restore original buffer for std::cout
            if (original_buffer_ != nullptr) {
                std::cout.rdbuf(original_buffer_);
            }
        }

      private:
        /// The TTY output stream.
        std::ofstream tty_{};

        /// The original buffer for std::cout before redirection.
        std::streambuf* original_buffer_{};
    };
}
