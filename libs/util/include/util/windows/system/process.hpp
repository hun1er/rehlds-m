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

#include <process.h>
#include <string>
#include <Windows.h>

namespace Util {
    /**
     * @brief Returns the canonical absolute path of the current executable.
     *
     * @return The canonical absolute path of the current executable.
     */
    [[nodiscard]] std::string get_executable_path();

    /**
     * @brief Returns the process ID of the calling process.
     *
     * @return The process ID of the calling process.
     */
    [[nodiscard]] inline int get_process_id()
    {
        return ::_getpid();
    }

    /**
     * @brief Retrieves the standard input handle or aborts the program.
     *
     * If the standard input handle cannot be retrieved, the program is aborted.
     *
     * @return The standard input handle.
     */
    [[nodiscard]] ::HANDLE get_stdin_handle_or_abort();

    /**
     * @brief Retrieves the standard output handle or aborts the program.
     *
     * If the standard output handle cannot be retrieved, the program is aborted.
     *
     * @return The standard output handle.
     */
    [[nodiscard]] ::HANDLE get_stdout_handle_or_abort();
}
