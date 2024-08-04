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

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <Windows.h>

namespace Util {
    /**
     * @brief Gets the device context for the specified window or aborts if an error occurs.
     *
     * @param window_handle A handle to the window for which to get the device context.
     *
     * @return A handle to the device context for the specified window.
     */
    [[nodiscard]] ::HDC get_device_context_or_abort(::HWND window_handle);

    /**
     * @brief Creates a new font object with the specified attributes or aborts if an error occurs.
     *
     * @param font_attributes A pointer to a LOGFONT structure that contains the font attributes.
     *
     * @return A handle to the new font object.
     */
    [[nodiscard]] ::HFONT create_font_or_abort(const ::LOGFONT& font_attributes);
}
