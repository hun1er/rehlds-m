/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "console/console_input.hpp"
#include "console/linux/terminal_settings.hpp"
#include "console/linux/tty_redirect.hpp"
#include "util/linux/system/error.hpp"
#include "util/logger.hpp"
#include <array>
#include <poll.h>
#include <string>
#include <unistd.h>

namespace
{
    /// Reads a character from the standard input.
    int read_char_from_stdin()
    {
        int input_char = '\0';
        const auto bytes_read = ::read(STDIN_FILENO, &input_char, 1);

        if ((0 == bytes_read) || ('\0' == input_char)) {
            return 0;
        }

        if (-1 == bytes_read) {
            util::log_error("Failed to read from terminal: {}", util::get_last_error_string());
            return 0;
        }

        return input_char;
    }
}

namespace view
{
    void ConsoleInput::handle_escape()
    {
        if (read_char_from_stdin() != '[') {
            return;
        }

        switch (read_char_from_stdin()) {
            case 'A': handle_up_arrow(); break;
            case 'B': handle_down_arrow(); break;
            case 'C': handle_right_arrow(); break;
            case 'D': handle_left_arrow(); break;
            case 'F': handle_end(); break;
            case 'H': handle_home(); break;
            case '3': {
                if ('~' == read_char_from_stdin()) {
                    handle_delete();
                }

                break;
            }
        }
    }

    void ConsoleInput::read_input()
    {
        const ScopedTerminalSettings terminal_settings{};
        const ScopedTtyRedirect tty_redirect{};

        std::array<::pollfd, 1> pfd{{{STDIN_FILENO, POLLIN, 0}}};
        constexpr ::nfds_t pfd_size = 1;
        constexpr auto timeout_msec = 100;

        while (running_) {
            if (::poll(pfd.data(), pfd_size, timeout_msec) <= 0) {
                continue;
            }

            switch (const auto input_char = read_char_from_stdin(); input_char) {
                case '\0': continue;
                case '\n': handle_newline(); break;
                case '\x1B': handle_escape(); break;
                case '\x7F':
                case '\b': handle_backspace(); break;
                case '\t': handle_tab(); break;
                default: handle_char(static_cast<std::string::value_type>(input_char)); break;
            }
        }
    }
}
