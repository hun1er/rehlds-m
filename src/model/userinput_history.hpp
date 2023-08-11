/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "util/circular_buffer.hpp"
#include <optional>
#include <string>
#include <string_view>

namespace model
{
    /** @brief The maximum capacity of the history buffer. */
    constexpr util::CircularBuffer<std::string, 0>::SizeType HISTORY_CAPACITY = 100;

    /**
     * @brief A class for storing and managing user input history.
     */
    class UserInputHistory
    {
      public:
        /**
         * @brief Type alias for the size type of the buffer.
         */
        using SizeType = util::CircularBuffer<std::string, 0>::SizeType;

        /**
         * @brief Construct a new UserInputHistory object.
         *
         * @param filename The name of the file to load and save history from/to.
         */
        explicit UserInputHistory(std::string_view filename = "");

        /**
         * @brief Append a new input to the history.
         *
         * @param input The input to append.
         */
        void append(std::string_view input);

        /**
         * @brief Get the next input in the history.
         *
         * @return The next input in the history, or an empty optional if there is no next input.
         */
        [[nodiscard]] std::optional<std::string> next() noexcept;

        /**
         * @brief Get the previous input in the history.
         *
         * @return The previous input in the history, or an empty optional if there is no previous input.
         */
        [[nodiscard]] std::optional<std::string> previous() noexcept;

        /**
         * @brief Clear the history.
         */
        void clear();

        /**
         * @brief Load the history from file.
         */
        void load();

        /**
         * @brief Save the history to file.
         */
        void save();

      private:
        /// The circular buffer to store the history.
        util::CircularBuffer<std::string, HISTORY_CAPACITY> history_{};

        /// The current position in the history buffer.
        SizeType position_{};

        /// The name of the file to load and save history from/to.
        std::string filename_{};
    };
}
