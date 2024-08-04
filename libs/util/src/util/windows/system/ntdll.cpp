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

#include "util/windows/system/ntdll.hpp"
#include "util/logger.hpp"
#include "util/windows/system/module.hpp"
#include <cstdlib>

namespace {
    ::HMODULE get_ntdll_handle()
    {
        static auto* const handle = Util::load_module<::HMODULE>("ntdll.dll");
        return handle;
    }
}

namespace Util::ntdll {
    ::NTSTATUS set_timer_resolution(
      const ::ULONG requested_resolution, const ::BOOLEAN set_resolution, ::ULONG* const actual_resolution)
    {
        using NtSetTimerResolution = ::NTSTATUS(__stdcall*)(::ULONG, ::BOOLEAN, ::PULONG);

        static auto* const nt_set_timer_resolution =
          Util::get_symbol_address<NtSetTimerResolution>(get_ntdll_handle(), "ZwSetTimerResolution");

        return nt_set_timer_resolution(requested_resolution, set_resolution, actual_resolution);
    }

    ::NTSTATUS set_timer_resolution(const ::ULONG requested_resolution, const ::BOOLEAN set_resolution)
    {
        ::ULONG actual_resolution{};
        return set_timer_resolution(requested_resolution, set_resolution, &actual_resolution);
    }

    void set_timer_resolution_or_abort(const ::ULONG requested_resolution, const ::BOOLEAN set_resolution)
    {
        if (const auto status = set_timer_resolution(requested_resolution, set_resolution); status != STATUS_SUCCESS) {
            log_critical("Failed to set timer resolution. Status code: {}", status);
            std::abort();
        }
    }

    ::NTSTATUS delay_execution(const ::BOOL alertable, ::_LARGE_INTEGER* const delay_interval)
    {
        using NtDelayExecution = ::NTSTATUS(__stdcall*)(::BOOL, ::PLARGE_INTEGER);

        static auto* const nt_delay_execution =
          Util::get_symbol_address<NtDelayExecution>(get_ntdll_handle(), "NtDelayExecution");

        return nt_delay_execution(alertable, delay_interval);
    }
}
