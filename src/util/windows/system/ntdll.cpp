/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/windows/system/ntdll.hpp"
#include "util/logger.hpp"
#include "util/windows/system/module.hpp"
#include <cstdlib>

namespace
{
    ::HMODULE get_ntdll_handle()
    {
        static auto* const handle = util::load_module<::HMODULE>("ntdll.dll");
        return handle;
    }
}

namespace util::ntdll
{
    ::NTSTATUS set_timer_resolution(const ::ULONG requested_resolution, const ::BOOLEAN set_resolution,
                                    ::ULONG* const actual_resolution)
    {
        using NtSetTimerResolution = ::NTSTATUS(__stdcall*)(::ULONG, ::BOOLEAN, ::PULONG);

        static auto* const nt_set_timer_resolution =
          util::get_symbol_address<NtSetTimerResolution>(get_ntdll_handle(), "ZwSetTimerResolution");

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
          util::get_symbol_address<NtDelayExecution>(get_ntdll_handle(), "NtDelayExecution");

        return nt_delay_execution(alertable, delay_interval);
    }
}
