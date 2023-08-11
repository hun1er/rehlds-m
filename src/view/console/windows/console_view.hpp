/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include "abstract/view.hpp"
#include <cstdio>
#include <fmt/core.h>
#include <string_view>
#include <Windows.h>

namespace view
{
    /**
     * @brief Displays information on the console.
     */
    class ConsoleView final : public abstract::View
    {
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

      private:
        /// A handle to the standard output stream.
        ::HANDLE stdout_handle_;

        /// A handle to the console window.
        ::HWND console_window_handle_;

        // A handle to the font object.
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
}
