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

#include "view/windows/console_view.hpp"
#include "util/logger.hpp"
#include "util/string.hpp"
#include "util/windows/console.hpp"
#include "util/windows/system/gdi.hpp"
#include "util/windows/system/process.hpp"

namespace {
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

        return Util::create_font_or_abort(font_attributes);
    }
}

namespace View {
    ConsoleView::ConsoleView() :
      stdout_handle_(Util::get_stdout_handle_or_abort()),
      console_window_handle_(Util::get_console_window_or_abort()),
      font_handle_(create_font_or_abort()),
      console_device_context_(Util::get_device_context_or_abort(console_window_handle_))
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

    void ConsoleView::display_server_status(
      const float fps, const int num_players, const int max_players, const std::string& map)
    {
        ::GetConsoleScreenBufferInfo(stdout_handle_, &screen_buffer_info);

        if (screen_buffer_info.srWindow.Top != current_scroll_pos_) {
            current_scroll_pos_ = screen_buffer_info.srWindow.Top;
            ::InvalidateRect(console_window_handle_, nullptr, TRUE);
        }
        else {
            auto status =
              Util::str::format("  FPS: {:.1f}   Players: {}/{}   Map: {}", fps, num_players, max_players, map);

            status = Util::str::ljust(status, screen_buffer_info.dwSize.X * 2);

            ::TextOut(console_device_context_, 1, 2, status.c_str(), static_cast<int>(status.length()));
        }
    }
}
