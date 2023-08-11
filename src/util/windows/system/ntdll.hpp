/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include <ntstatus.h>
#include <windows.h>
#include <winternl.h>

namespace util::ntdll
{
    /**
     * @brief Sets the timer resolution of the system.
     *
     * @param requested_resolution The requested timer resolution, in 100-nanosecond units.
     * @param set_resolution A boolean value indicating whether to set or clear the requested timer resolution.
     * @param actual_resolution A pointer to a variable that receives the actual timer resolution set,
     * in 100-nanosecond units.
     *
     * @return An NTSTATUS code indicating the success or failure of the operation.
     */
    ::NTSTATUS set_timer_resolution(::ULONG requested_resolution, ::BOOLEAN set_resolution, ::PULONG actual_resolution);

    /**
     * @brief Sets the timer resolution of the system.
     *
     * @param requested_resolution The requested timer resolution, in 100-nanosecond units.
     * @param set_resolution A boolean value indicating whether to set or clear the requested timer resolution.
     *
     * @return An NTSTATUS code indicating the success or failure of the operation.
     */
    ::NTSTATUS set_timer_resolution(::ULONG requested_resolution, ::BOOLEAN set_resolution = TRUE);

    /**
     * @brief Sets the timer resolution or aborts the program if the operation fails.
     *
     * @param requested_resolution The requested timer resolution, in 100-nanosecond units.
     * @param set_resolution A boolean value indicating whether to set or reset the timer resolution.
     *
     * If the operation fails, the program is aborted.
     */
    void set_timer_resolution_or_abort(::ULONG requested_resolution, ::BOOLEAN set_resolution = TRUE);

    /**
     * @brief Delays the execution of the calling thread for a specified interval.
     *
     * @param alertable A boolean value indicating whether the delay is alertable.
     * @param delay_interval A pointer to a LARGE_INTEGER structure that specifies the delay interval,
     * in 100-nanosecond units.
     *
     * @return An NTSTATUS code indicating the success or failure of the operation.
     */
    ::NTSTATUS delay_execution(::BOOL alertable, ::PLARGE_INTEGER delay_interval);
}
