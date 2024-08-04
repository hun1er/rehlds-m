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

#include "util/circular_buffer.hpp"
#include <optional>
#include <string>
#include <string_view>

namespace Model {
    /** @brief The maximum capacity of the history buffer. */
    constexpr Util::CircularBuffer<std::string, 0>::SizeType HISTORY_CAPACITY = 100;

    /**
     * @brief A class for storing and managing user input history.
     */
    class UserInputHistory {
      public:
        /**
         * @brief Type alias for the size type of the buffer.
         */
        using SizeType = Util::CircularBuffer<std::string, 0>::SizeType;

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
        Util::CircularBuffer<std::string, HISTORY_CAPACITY> history_{};

        /// The current position in the history buffer.
        SizeType position_{};

        /// The name of the file to load and save history from/to.
        std::string filename_{};
    };
}
