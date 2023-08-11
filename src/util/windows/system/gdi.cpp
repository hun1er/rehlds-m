/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/windows/system/gdi.hpp"
#include "util/logger.hpp"
#include <cstdlib>

namespace util
{
    ::HDC get_device_context_or_abort(::HWND__* const window_handle)
    {
        auto* const device_context = ::GetDC(window_handle);

        if (nullptr == device_context) {
            log_critical("Failed to get device context for window.");
            std::abort();
        }

        return device_context;
    }

    ::HFONT create_font_or_abort(const ::LOGFONT& font_attributes)
    {
        auto* const font_handle = ::CreateFontIndirect(&font_attributes);

        if (nullptr == font_handle) {
            log_critical("Failed to create font object.");
            std::abort();
        }

        return font_handle;
    }
}
