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
#include <cstdio>
#include <fstream>
#include <iostream>
#include <unistd.h>

namespace View {
    /**
     * @brief A RAII class for redirecting TTY output to \c std::cout within a scope.
     */
    class ScopedTtyRedirect final {
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
                Util::log_error("Failed to open TTY '{}' for output.", term_name);
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
