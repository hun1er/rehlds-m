/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <string>

namespace model
{
    /**
     * @brief Represents the current status of a server.
     *
     * Contains information about the current status of a server.
     */
    struct ServerStatus
    {
        /**
         * @brief The current FPS of the server.
         */
        float fps{};

        /**
         * @brief The number of active players on the server.
         */
        int num_players{};

        /**
         * @brief The maximum number of players allowed on the server.
         */
        int max_players{};

        /**
         * @brief The name of the current map on the server.
         */
        std::string current_map{};
    };
}
