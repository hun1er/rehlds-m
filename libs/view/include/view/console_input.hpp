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

#include <atomic>
#include <memory>
#include <optional>
#include <string>
#include <thread>

namespace Presenter {
    class InputPresenter;
}

namespace View {
    /**
     * @brief Handles console input.
     */
    class ConsoleInput final {
      public:
        /**
         * @brief Type alias for a shared pointer to an \c Presenter::InputPresenter
         */
        using InputPresenterPtr = std::shared_ptr<Presenter::InputPresenter>;

        /**
         * @brief Constructs a new ConsoleInput object.
         *
         * @param input_presenter A shared pointer to an \c Presenter::InputPresenter object.
         */
        explicit ConsoleInput(InputPresenterPtr input_presenter);

        /// Move constructor.
        ConsoleInput(ConsoleInput&&) = delete;

        /// Copy constructor.
        ConsoleInput(const ConsoleInput&) = delete;

        /// Move assignment operator.
        ConsoleInput& operator=(ConsoleInput&&) = delete;

        /// Copy assignment operator.
        ConsoleInput& operator=(const ConsoleInput&) = delete;

        /**
         * @brief Destructor.
         */
        ~ConsoleInput();

        /**
         * @brief Starts the input worker thread.
         */
        void start();

        /**
         * @brief Stops the input worker thread.
         */
        void stop();

      private:
        /// A shared pointer to an \c Presenter::InputPresenter
        InputPresenterPtr input_presenter_;

        /// Indicates whether the input worker thread is running.
        std::atomic<bool> running_{};

        /// The input worker thread.
        std::thread input_thread_{};

        /// The current input line.
        std::string input_line_{};

        /// Saved input line.
        std::optional<std::string> saved_input_{};

        /// The current cursor position in the input line.
        std::string::size_type cursor_position_{};

#ifndef _WIN32
        /// Handles ANSI escape codes.
        void handle_escape();
#endif
        /// Handles the up arrow key.
        void handle_up_arrow();

        /// Handles the down arrow key.
        void handle_down_arrow();

        /// Handles the left arrow key.
        void handle_left_arrow();

        /// Handles the right arrow key.
        void handle_right_arrow();

        /// Handles the backspace key.
        void handle_backspace();

        /// Handles the delete key.
        void handle_delete();

        /// Handles the home key.
        void handle_home();

        /// Handles the end key.
        void handle_end();

        /// Handles the tab key.
        void handle_tab();

        /// Handles the newline character (Enter key).
        void handle_newline();

        /// Handles a character input.
        void handle_char(std::string::value_type ch);

        /// Reads input from the console (runs in a separate thread).
        void read_input();

        /// Clears the current input line from the console.
        void clear_input();
    };
}
