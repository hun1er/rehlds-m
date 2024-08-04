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

#include <array>
#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <Windows.h>

namespace Util {
    /**
     * @brief Opaque handle to the module.
     */
    class SysModule;

    /**
     * @brief Returns the canonical absolute path for the given module filename.
     *
     * The input is a string representing the filename of a module.
     * The output is the canonical absolute path to that module
     * by appending the current working directory to the filename.
     *
     * @param filename The filename of the module to be converted to a canonical absolute path.
     *
     * @return The canonical absolute path for the given module filename.
     */
    [[nodiscard]] std::string get_module_canonical_path(std::string_view filename);

    /**
     * @brief Returns the filename of the specified module.
     *
     * Takes a handle to a module as an argument. Returns the filename of the module
     * associated with that handle as a \c std::filesystem::path object.
     * If the \p module parameter is \c nullptr, the function returns the filename
     * of the executable file of the current process.
     *
     * @tparam Handle The type of the handle to be passed as an argument.
     *
     * @param module The handle to the module whose filename is to be returned.
     *
     * @return The filename of the specified module or an empty object if an error occurs.
     */
    template <typename Handle = ::HMODULE>
    [[nodiscard]] std::filesystem::path get_module_filename(Handle&& module)
    {
        std::array<::CHAR, MAX_PATH> name{};
        ::DWORD name_len{};

        if constexpr (std::is_null_pointer_v<Handle>) {
            name_len = ::GetModuleFileName(nullptr, name.data(), name.size() - 1);
        }
        else if constexpr (std::is_same_v<Handle, ::HMODULE>) {
            name_len = ::GetModuleFileName(std::forward<Handle>(module), name.data(), name.size() - 1);
        }
        else {
            name_len = // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
              ::GetModuleFileName(reinterpret_cast<::HMODULE>(std::forward<Handle>(module)), name.data(), name.size());
        }

        return NULL == name_len ? std::filesystem::path{} : std::filesystem::path{name.data()};
    }

    /**
     * @brief Loads the specified module and returns a handle to it.
     *
     * Takes the filename of a module as an argument.
     * Returns a handle to the loaded module.
     *
     * @tparam Ret The type of the handle to be returned.
     *
     * @param filename The filename of the module to be loaded.
     *
     * @return A handle to the loaded module, or \c nullptr if an error occurs.
     */
    template <typename Ret = SysModule*>
    [[nodiscard]] Ret load_module(const std::string& filename) noexcept
    {
        const auto& canonical_path = get_module_canonical_path(filename);
        auto* const handle = ::LoadLibrary(canonical_path.empty() ? filename.c_str() : canonical_path.c_str());

        if constexpr (std::is_same_v<Ret, ::HMODULE>) {
            return handle;
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(handle);
        }
    }

    /**
     * @brief Unloads the specified module.
     *
     * Takes a handle to a loaded module as an argument and unloads it.
     *
     * @param module The handle to the module to be unloaded.
     *
     * @return \c true if the module was successfully unloaded, \c false otherwise.
     */
    template <typename Handle>
    bool unload_module(Handle&& module) noexcept
    {
        if constexpr (std::is_same_v<Handle, ::HMODULE>) {
            return ::FreeLibrary(std::forward<::HMODULE>(module)) != FALSE;
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return ::FreeLibrary(reinterpret_cast<::HMODULE>(std::forward<Handle>(module))) != FALSE;
        }
    }

    /**
     * @brief Returns the address of the specified symbol in the given module.
     *
     * Takes a handle to a loaded module and the name of a symbol as arguments.
     * Returns the address of the specified symbol in the given module.
     *
     * @tparam Ret The type of the address to be returned.
     * @tparam Handle The type of the handle to the loaded module.
     *
     * @param module_handle The handle to the loaded module.
     * @param symbol_name The name of the symbol to be looked up.
     *
     * @return The address of the specified symbol in the given module, or \c nullptr if an error occurs.
     */
    template <typename Ret = void*, typename Handle>
    [[nodiscard]] Ret get_symbol_address(Handle&& module_handle, const std::string& symbol_name) noexcept
    {
        const auto address =
          ::GetProcAddress(reinterpret_cast<::HMODULE>(std::forward<Handle>(module_handle)), symbol_name.c_str());

        if constexpr (std::is_same_v<Ret, void*>) {
            return address;
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(address);
        }
    }
}
