/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)

#include "util/windows/system/crash_dumper.hpp"
#include "util/logger.hpp"
#include "util/windows/system/error.hpp"
#include "util/windows/system/module.hpp"
#include <cstdlib>
#include <DbgHelp.h>
#include <Windows.h>

namespace
{
    /// Handle of the dbghelp.dll library.
    ::HMODULE dbghelp{};

    /// Type alias for type of the MiniDumpWriteDump function pointer.
    using MiniDumpWriteDumpFunc = decltype(&::MiniDumpWriteDump);

    /// Address of the MiniDumpWriteDump function.
    MiniDumpWriteDumpFunc mini_dump_write_dump{};

    /// Previous exception filter.
    ::LPTOP_LEVEL_EXCEPTION_FILTER previous_exception_filter{};

    /// Creates a unique file name for the dump file.
    std::string create_dump_filename()
    {
        ::SYSTEMTIME sys_time{};
        ::GetLocalTime(&sys_time);

        auto exe_name = util::get_module_filename(nullptr);
        exe_name = exe_name.empty() ? "unknown" : exe_name.stem();

        return fmt::format("{}_crash_{}{:02d}{:02d}_{:02d}{:02d}{:02d}.mdmp", exe_name.string(), sys_time.wYear,
                           sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond);
    }

    /// Writes a dump file with minimal or maximal information.
    ::BOOL write_dump(void* const file, ::EXCEPTION_POINTERS* const exception_info)
    {
        ::MINIDUMP_EXCEPTION_INFORMATION mei{};
        mei.ThreadId = ::GetCurrentThreadId();
        mei.ExceptionPointers = exception_info;
        mei.ClientPointers = FALSE;

        auto* const process_handle = ::GetCurrentProcess();
        const auto process_id = ::GetCurrentProcessId();
        const auto minidump_type = static_cast<::MINIDUMP_TYPE>(
          MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithIndirectlyReferencedMemory |
          MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo | MiniDumpWithCodeSegs | MiniDumpWithModuleHeaders);

        // Attempt to write a mini-dump with the maximum amount of information about the process (i.e., a large file).
        // If that fails, write a more compact mini-dump with less information.
        if (FALSE == mini_dump_write_dump(process_handle, process_id, file, minidump_type, &mei, nullptr, nullptr)) {
            return mini_dump_write_dump(process_handle, process_id, file, MiniDumpWithDataSegs, &mei, nullptr, nullptr);
        }

        return TRUE;
    }

    /// Handles unhandled exceptions by creating a dump file.
    ::LONG WINAPI unhandled_exception_filter(::_EXCEPTION_POINTERS* const exception_info)
    {
        if ((nullptr == dbghelp) || (nullptr == mini_dump_write_dump)) {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        const auto& filename = create_dump_filename();

        auto* const file_handle =
          ::CreateFile(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        if (INVALID_HANDLE_VALUE == file_handle) {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        const auto success = write_dump(file_handle, exception_info);
        ::CloseHandle(file_handle);

        return TRUE == success ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
    }
}

namespace util
{
    CrashDumper::CrashDumper()
    {
        if (dbghelp = load_module<::HMODULE>("dbghelp.dll"); nullptr == dbghelp) {
            log_critical("Failed to load dbghelp.dll: {}", get_last_error_string());
            std::abort();
        }

        if (mini_dump_write_dump = get_symbol_address<MiniDumpWriteDumpFunc>(dbghelp, "MiniDumpWriteDump");
            nullptr == mini_dump_write_dump) {
            log_critical("Failed to get the address of MiniDumpWriteDump function: {}", get_last_error_string());
            std::abort();
        }

        previous_exception_filter = ::SetUnhandledExceptionFilter(unhandled_exception_filter);
    }

    CrashDumper::~CrashDumper()
    {
        ::SetUnhandledExceptionFilter(previous_exception_filter);
        unload_module(dbghelp);
        dbghelp = nullptr;
    }

    void CrashDumper::init()
    {
        [[maybe_unused]] const auto& instance = SingletonHolder<CrashDumper>::get_instance();
    }
}
