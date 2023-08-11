/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <dlfcn.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace util
{
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
     * @brief Loads the specified module and returns a handle to it.
     *
     * Takes the filename of a module and a mode flag as arguments.
     * Returns a handle to the loaded module.
     *
     * @tparam Ret The type of the handle to be returned.
     *
     * @param filename The filename of the module to be loaded.
     * @param mode The mode flag for loading the module.
     *
     * @return A handle to the loaded module, or \c nullptr if an error occurs.
     */
    template <typename Ret = SysModule*>
    [[nodiscard]] Ret load_module(const std::string& filename, const int mode = RTLD_NOW)
    {
        const auto& canonical_path = get_module_canonical_path(filename);
        auto* const handle = ::dlopen(canonical_path.empty() ? filename.c_str() : canonical_path.c_str(), mode);

        if constexpr (std::is_same_v<Ret, void*>) {
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
    template <typename Module = SysModule>
    bool unload_module(Module&& module) noexcept
    {
        return 0 == ::dlclose(std::forward<Module>(module));
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
        auto* const address = ::dlsym(std::forward<Handle>(module_handle), symbol_name.c_str());

        if constexpr (std::is_same_v<Ret, void*>) {
            return address;
        }
        else {
            // NOLINTNEXTLINE(clang-diagnostic-cast-function-type)
            return reinterpret_cast<Ret>(address);
        }
    }
}
