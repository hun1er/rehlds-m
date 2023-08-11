/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include "console/console_input.hpp"
#include "util/logger.hpp"
#include "util/system.hpp"
#include <chrono>
#include <Windows.h>

namespace
{
    bool has_input(void* const stdin_handle, ::INPUT_RECORD& input_record) noexcept
    {
        auto num_events_read = DWORD{0};

        if (FALSE == ::PeekConsoleInput(stdin_handle, &input_record, 1, &num_events_read)) {
            util::log_error("Failed to peek console input: {}", util::get_last_error_string());
            return false;
        }

        return num_events_read > 0;
    }

    bool read_console_input(void* const stdin_handle, ::INPUT_RECORD& input_record) noexcept
    {
        auto num_events_read = DWORD{0};

        if (FALSE == ::ReadConsoleInput(stdin_handle, &input_record, 1, &num_events_read)) {
            util::log_error("Failed to read from terminal: {}", util::get_last_error_string());
        }

        return num_events_read > 0;
    }

    bool is_key_event(const ::INPUT_RECORD& input_record) noexcept
    {
        return input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown != FALSE;
    }
}

namespace view
{
    void ConsoleInput::read_input()
    {
        ::INPUT_RECORD input_record{};
        auto* const stdin_handle = util::get_stdin_handle_or_abort();
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
