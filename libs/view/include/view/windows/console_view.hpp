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

#define WIN32_LEAN_AND_MEAN

#include "util/log_output.hpp"
#include "view/base_view.hpp"
#include <fmt/core.h>
#include <cstdio>
#include <string_view>
#include <Windows.h>

namespace View {
    /**
     * @brief Displays information on the console.
     */
    class ConsoleView final : public BaseView, public Util::LogOutput {
      public:
        /**
         * @brief Default constructor.
         */
        ConsoleView();

        /**
         * @brief Move constructor.
         */
        ConsoleView(ConsoleView&&) = default;

        /**
         * @brief Copy constructor.
         */
        ConsoleView(const ConsoleView&) = default;

        /**
         * @brief Move assignment operator.
         */
        ConsoleView& operator=(ConsoleView&&) = default;

        /**
         * @brief Copy assignment operator.
         */
        ConsoleView& operator=(const ConsoleView&) = default;

        /**
         * @brief Destructor.
         */
        ~ConsoleView() override;

        /**
         * @brief Prints the given text.
         *
         * @param text The text to be printed.
         *
         * @return 0 if the operation is successful.
         */
        int print(std::string_view text) override; // NOLINT(modernize-use-nodiscard)

        /**
         * @brief Displays the server status information.
         *
         * @param fps The current FPS of the server.
         * @param num_players The number of active players on the server.
         * @param max_players The maximum number of players allowed on the server.
         * @param map The name of the current map on the server.
         */
        void display_server_status(float fps, int num_players, int max_players, const std::string& map) override;

        /**
         * @brief Write a log message to the console.
         *
         * @param message The formatted log message to be written.
         */
        void write_log(std::string_view message) override;

      private:
        /// A handle to the standard output stream.
        ::HANDLE stdout_handle_;

        /// A handle to the console window.
        ::HWND console_window_handle_;

        /// A handle to the font object.
        ::HFONT font_handle_;

        /// A handle to the device context of the console window.
        ::HDC console_device_context_;

        /// The current scroll position of the console window.
        ::SHORT current_scroll_pos_{};
    };

    inline int ConsoleView::print(const std::string_view text)
    {
        fmt::print(text);

        return std::fflush(stdout);
    }

    inline void ConsoleView::write_log(const std::string_view message)
    {
        print(message);
    }
}
