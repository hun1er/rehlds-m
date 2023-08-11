/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "console/windows/console_view.hpp"
#include "util/logger.hpp"
#include "util/string.hpp"
#include "util/windows/console.hpp"
#include "util/windows/system/gdi.hpp"
#include "util/windows/system/process.hpp"

namespace
{
    /// Information about the console screen buffer.
    ::CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info{};

    /// Creates a new font object or aborts if an error occurs.
    ::HFONT create_font_or_abort()
    {
        ::LOGFONT font_attributes{};
        font_attributes.lfHeight = 21;
        font_attributes.lfWeight = FW_BOLD;
        font_attributes.lfCharSet = ANSI_CHARSET;
        ::lstrcpy(font_attributes.lfFaceName, TEXT("Arial"));

        return util::create_font_or_abort(font_attributes);
    }
}

namespace view
{
    ConsoleView::ConsoleView() :
      stdout_handle_(util::get_stdout_handle_or_abort()),
      console_window_handle_(util::get_console_window_or_abort()),
      font_handle_(create_font_or_abort()),
      console_device_context_(util::get_device_context_or_abort(console_window_handle_))
    {
        ::SetBkColor(console_device_context_, RGB(0, 0, 0));       // Background color
        ::SetTextColor(console_device_context_, RGB(50, 205, 50)); // Text color
        ::SelectObject(console_device_context_, font_handle_);     // Font
    }

    ConsoleView::~ConsoleView()
    {
        ::ReleaseDC(console_window_handle_, console_device_context_);
        ::DeleteObject(font_handle_);
    }

    void ConsoleView::display_server_status(const float fps, const int num_players, const int max_players,
                                            const std::string& map)
    {
        ::GetConsoleScreenBufferInfo(stdout_handle_, &screen_buffer_info);

        if (screen_buffer_info.srWindow.Top != current_scroll_pos_) {
            current_scroll_pos_ = screen_buffer_info.srWindow.Top;
            ::InvalidateRect(console_window_handle_, nullptr, TRUE);
        }
        else {
            auto status =
              util::str::format("  FPS: {:.1f}   Players: {}/{}   Map: {}", fps, num_players, max_players, map);

            status = util::str::ljust(status, screen_buffer_info.dwSize.X * 2);

            ::TextOut(console_device_context_, 1, 2, status.c_str(), static_cast<int>(status.length()));
        }
    }
}
