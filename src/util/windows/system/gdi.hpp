/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <Windows.h>

namespace util
{
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
