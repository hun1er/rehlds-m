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

#include "model/console_commands.hpp"
#include "model/server_loop.hpp"
#include "model/userinput_history.hpp"
#include <cassert>
#include <memory>
#include <utility>

namespace Presenter {
    /**
     * @brief A presenter class that handles user input and input history in an MVP pattern.
     */
    class InputPresenter final {
      public:
        /**
         * @brief Type alias for a shared pointer to a ServerLoop object.
         */
        using ServerLoopPtr = std::shared_ptr<Model::ServerLoop>;

        /**
         * @brief Type alias for a shared pointer to a UserInputHistory object.
         */
        using InputHistoryPtr = std::shared_ptr<Model::UserInputHistory>;

        /**
         * @brief Type alias for a shared pointer to a ConsoleCommands object.
         */
        using ConsoleCommandsPtr = std::shared_ptr<Model::ConsoleCommands>;

        /**
         * @brief Constructs an InputPresenter object.
         *
         * @param server_loop A shared pointer to a ServerLoop object.
         * @param input_history A shared pointer to a UserInputHistory object.
         * @param console_commands A shared pointer to a ConsoleCommands object.
         */
        InputPresenter(ServerLoopPtr server_loop, InputHistoryPtr input_history, ConsoleCommandsPtr console_commands);

        /**
         * @brief Enqueues user input to the ServerLoop and adds it to the input history.
         *
         * @param input The user input to enqueue and add to the history.
         */
        void enqueue_input(std::string_view input) const;

        /**
         * @brief Searches for console command matches for the given command.
         *
         * @param command The command to search for matches.
         *
         * @return A vector of matching console commands.
         */
        [[nodiscard]] std::vector<std::string> find_command_matches(std::string_view command) const;

        /**
         * @brief Retrieves the previous entry in the input history.
         *
         * @return An optional string containing the previous entry in the input history,
         * or an empty optional if there is no previous entry.
         */
        [[nodiscard]] std::optional<std::string> history_previous() const;

        /**
         * @brief Retrieves the next entry in the input history.
         *
         * @return An optional string containing the next entry in the input history,
         * or an empty optional if there is no next entry.
         */
        [[nodiscard]] std::optional<std::string> history_next() const;

      private:
        /// A shared pointer to a ServerLoop object.
        ServerLoopPtr server_loop_{};

        /// A shared pointer to a UserInputHistory object.
        InputHistoryPtr input_history_{};

        /// A shared pointer to a ConsoleCommands object.
        ConsoleCommandsPtr console_commands_{};
    };

    inline InputPresenter::InputPresenter(
      ServerLoopPtr server_loop, InputHistoryPtr input_history, ConsoleCommandsPtr console_commands) :
      server_loop_(std::move(server_loop)),
      input_history_(std::move(input_history)),
      console_commands_(std::move(console_commands))
    {
        assert(server_loop_ != nullptr);
        assert(input_history_ != nullptr);
        assert(console_commands_ != nullptr);
        input_history_->load();
    }

    inline void InputPresenter::enqueue_input(const std::string_view input) const
    {
        server_loop_->enqueue_input(input);
        input_history_->append(input);
        input_history_->save();
    }

    inline std::vector<std::string> InputPresenter::find_command_matches(const std::string_view command) const
    {
        auto matches = server_loop_->enqueue_task<std::vector<std::string>>([this, command] {
            return console_commands_->find_command_matches(std::string{command});
        });

        return matches.get();
    }

    inline std::optional<std::string> InputPresenter::history_previous() const
    {
        return input_history_->previous();
    }

    inline std::optional<std::string> InputPresenter::history_next() const
    {
        return input_history_->next();
    }
}
