/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "abstract/view.hpp"
#include <fmt/core.h>

namespace view
{
    class ConsoleView final : public abstract::View
    {
      public:
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
    };

    inline void ConsoleView::display_server_status(const float /* fps */, const int /* num_players */,
                                                   const int /* max_players */, const std::string& /* map */)
    {
        // No implementation on Linux
    }

    inline int ConsoleView::print(const std::string_view text)
    {
        fmt::print(text);
        return std::fflush(stdout);
    }
}
