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

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include "view/console_input.hpp"
#include "util/logger.hpp"
#include "util/system.hpp"
#include <chrono>
#include <Windows.h>

namespace {
    bool has_input(void* const stdin_handle, ::INPUT_RECORD& input_record) noexcept
    {
        auto num_events_read = DWORD{0};

        if (FALSE == ::PeekConsoleInput(stdin_handle, &input_record, 1, &num_events_read)) {
            Util::log_error("Failed to peek console input: {}", Util::get_last_error_string());
            return false;
        }

        return num_events_read > 0;
    }

    bool read_console_input(void* const stdin_handle, ::INPUT_RECORD& input_record) noexcept
    {
        auto num_events_read = DWORD{0};

        if (FALSE == ::ReadConsoleInput(stdin_handle, &input_record, 1, &num_events_read)) {
            Util::log_error("Failed to read from terminal: {}", Util::get_last_error_string());
        }

        return num_events_read > 0;
    }

    bool is_key_event(const ::INPUT_RECORD& input_record) noexcept
    {
        return input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown != FALSE;
    }
}

namespace View {
    void ConsoleInput::read_input()
    {
        ::INPUT_RECORD input_record{};
        auto* const stdin_handle = Util::get_stdin_handle_or_abort();
        constexpr auto sleep_time = std::chrono::milliseconds{25};

        while (running_) {
            if ((!has_input(stdin_handle, input_record)) || (!read_console_input(stdin_handle, input_record))) {
                std::this_thread::sleep_for(sleep_time);
                continue;
            }

            if (!is_key_event(input_record)) {
                continue;
            }

            const auto key_code = input_record.Event.KeyEvent.wVirtualKeyCode;
            const auto character = input_record.Event.KeyEvent.uChar.AsciiChar;

            if ((VK_RETURN == key_code) || ('\r' == character) || ('\n' == character)) {
                handle_newline();
                continue;
            }

            switch (key_code) {
                case VK_UP: handle_up_arrow(); continue;
                case VK_DOWN: handle_down_arrow(); continue;
                case VK_LEFT: handle_left_arrow(); continue;
                case VK_RIGHT: handle_right_arrow(); continue;
                case VK_BACK: handle_backspace(); continue;
                case VK_DELETE: handle_delete(); continue;
                case VK_HOME: handle_home(); continue;
                case VK_END: handle_end(); continue;
                case VK_TAB: handle_tab(); continue;
                default: break;
            }

            switch (character) {
                case '\0': break;
                case '\t': handle_tab(); break;
                case '\b':
                case '\x7F': handle_backspace(); break;
                default: handle_char(character); break;
            }
        }
    }
}
