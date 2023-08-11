/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <string>
#include <string_view>

namespace abstract
{
    /**
     * @brief An abstract interface for displaying information.
     */
    class View
    {
      public:
        /**
         * @brief Default constructor.
         */
        View() = default;

        /**
         * @brief Move constructor.
         */
        View(View&&) = default;

        /**
         * @brief Copy constructor.
         */
        View(const View&) = default;

        /**
         * @brief Move assignment operator.
         */
        View& operator=(View&&) = default;

        /**
         * @brief Copy assignment operator.
         */
        View& operator=(const View&) = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~View() = default;

        /**
         * @brief Prints the given text.
         *
         * @param text The text to be printed.
         *
         * @return 0 if the operation is successful.
         */
        virtual int print(std::string_view text) = 0;

        /**
         * @brief Displays the server status information.
         *
         * @param fps The current FPS of the server.
         * @param num_players The number of active players on the server.
         * @param max_players The maximum number of players allowed on the server.
         * @param map The name of the current map on the server.
         */
        virtual void display_server_status(float fps, int num_players, int max_players, const std::string& map) = 0;
    };
}
