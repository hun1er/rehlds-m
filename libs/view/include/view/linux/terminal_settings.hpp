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
#include "util/signal.hpp"
#include "util/system.hpp"
#include <cstring>
#include <termios.h>
#include <unistd.h>

namespace View {
    /**
     * @brief A RAII class for managing terminal settings within a scope.
     */
    class ScopedTerminalSettings final {
      public:
        /**
         * @brief Construct a new ScopedTerminalSettings object and modify the terminal settings.
         */
        ScopedTerminalSettings()
        {
            // Block the SIGTTIN and SIGTTOU signals to prevent the process from being stopped
            const Util::ScopedSignalBlocker signal_blocker{SIGTTIN, SIGTTOU};

            // Check if the standard input is connected to a terminal
            if (is_stdin_terminal_ = (::isatty(STDIN_FILENO) != 0); !is_stdin_terminal_) {
                return;
            }

            // Get the current terminal attributes and save them
            if (0 == ::tcgetattr(STDIN_FILENO, &original_attributes_)) {
                std::memcpy(&modified_attributes_, &original_attributes_, sizeof(termios));
            }
            else {
                Util::log_error("Failed to get terminal attributes: {}", Util::get_last_error_string());
            }

            // Disable canonical mode, echo, and set the buffer size to 1 byte
            modified_attributes_.c_lflag &= ~static_cast<::tcflag_t>(ICANON | ECHO);
            modified_attributes_.c_cc[VMIN] = 1;
            modified_attributes_.c_cc[VTIME] = 0;

            // Set new terminal attributes
            if (::tcsetattr(STDIN_FILENO, TCSANOW, &modified_attributes_) != 0) {
                Util::log_error("Failed to set new terminal attributes: {}", Util::get_last_error_string());
            }
        }

        /// Move constructor.
        ScopedTerminalSettings(ScopedTerminalSettings&&) = delete;

        /// Copy constructor.
        ScopedTerminalSettings(const ScopedTerminalSettings&) = delete;

        /// Move assignment operator.
        ScopedTerminalSettings& operator=(ScopedTerminalSettings&&) = delete;

        /// Copy assignment operator.
        ScopedTerminalSettings& operator=(const ScopedTerminalSettings&) = delete;

        /**
         * @brief Destroy the ScopedTerminalSettings object and restore the original terminal settings.
         */
        ~ScopedTerminalSettings()
        {
            if (!is_stdin_terminal_) {
                return;
            }

            // Block the SIGTTIN and SIGTTOU signals to prevent the process from being stopped
            const Util::ScopedSignalBlocker signal_blocker{SIGTTIN, SIGTTOU};

            // Restore original terminal attributes
            if (::tcsetattr(STDIN_FILENO, TCSANOW, &original_attributes_) != 0) {
                Util::log_error("Failed to restore original terminal attributes: {}", Util::get_last_error_string());
            }
        }

      private:
        /// Whether the standard input is connected to a terminal.
        bool is_stdin_terminal_{};

        /// The original terminal attributes before modification.
        ::termios original_attributes_{};

        /// The modified terminal attributes.
        ::termios modified_attributes_{};
    };
}
