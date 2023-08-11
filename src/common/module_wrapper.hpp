/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "util/logger.hpp"
#include "util/system.hpp"
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>

namespace common
{
    /**
     * @brief A wrapper class for managing modules.
     */
    class ModuleWrapper
    {
      public:
        /**
         * @brief Constructs a new ModuleWrapper object.
         *
         * @param name The name of the dynamic library to manage.
         */
        explicit ModuleWrapper(std::string name);

        /// Move constructor.
        ModuleWrapper(ModuleWrapper&&) = delete;

        /// Copy constructor.
        ModuleWrapper(const ModuleWrapper&) = delete;

        /// Move assignment operator.
        ModuleWrapper& operator=(ModuleWrapper&&) = delete;

        /// Copy assignment operator.
        ModuleWrapper& operator=(const ModuleWrapper&) = delete;

        /**
         * @brief Virtual destructor.
         */
        virtual ~ModuleWrapper();

        /**
         * @brief Checks if the module is loaded.
         *
         * @return \c true if the module is loaded, \c false otherwise.
         */
        [[nodiscard]] bool is_loaded() const noexcept;

        /**
         * @brief Loads the module.
         *
         * @return \c true if the module was loaded successfully, \c false otherwise.
         */
        bool load();

        /**
         * @brief Loads the module or aborts the program.
         *
         * Attempts to load the module. If the module fails to load,
         * the function calls \c std::abort to abort the program.
         */
        void load_or_abort();

        /**
         * @brief Unloads the module.
         */
        void unload() noexcept;

        /**
         * @brief Gets the address of the \c CreateInterface function from the module.
         *
         * @return The address of the \c CreateInterface function, or \c nullptr if not found.
         */
        [[nodiscard]] CreateInterfaceFunc get_interface_factory() const;

        /**
         * @brief Gets the address of a symbol from the module.
         *
         * @tparam T The type of the symbol to retrieve.
         *
         * @param symbol_name The name of the symbol to retrieve.
         *
         * @return The address of the symbol, or \c nullptr if not found.
         */
        template <typename T = void*>
        [[nodiscard]] T get_symbol_address(const std::string& symbol_name) const;

        /**
         * @brief Gets an interface from the module using its name.
         *
         * @tparam T The type of the interface to retrieve.
         *
         * @param name The name of the interface to retrieve.
         * @param cache Whether to cache the retrieved interface.
         *
         * @return A pointer to the retrieved interface, or nullptr if not found or failed to retrieve.
         */
        template <typename T>
        [[nodiscard]] T* get_interface(const std::string& name, bool cache = true);

        /**
         * @brief Retrieves an interface from the module using its name and aborts if the interface retrieval fails.
         *
         * @tparam T The type of the interface to retrieve.
         *
         * @param name The name of the interface to retrieve.
         * @param cache Whether to cache the retrieved interface.
         *
         * @return A pointer to the retrieved interface. If the interface retrieval fails, the program will abort.
         */
        template <typename T>
        [[nodiscard]] T* get_interface_or_abort(const std::string& name, bool cache = true);

      private:
        /// The name of the module to manage
        std::string name_;

        /// A handle to the loaded module
        util::SysModule* handle_{};

        /// A cache of retrieved interfaces
        std::unordered_map<std::string, CommonInterface*> interfaces_{};
    };

    inline bool ModuleWrapper::is_loaded() const noexcept
    {
        return handle_ != nullptr;
    }

    inline CreateInterfaceFunc ModuleWrapper::get_interface_factory() const
    {
        return get_symbol_address<CreateInterfaceFunc>(CREATE_INTERFACE_SYMBOL);
    }

    template <typename T>
    T ModuleWrapper::get_symbol_address(const std::string& symbol_name) const
    {
        return util::get_symbol_address<T>(handle_, symbol_name);
    }

    template <typename T>
    T* ModuleWrapper::get_interface(const std::string& name, const bool cache)
    {
        if (const auto& it = interfaces_.find(name); it != interfaces_.end()) {
            return static_cast<T*>(it->second);
        }

        auto status = CreateInterfaceStatus::failed;
        auto* const factory = get_interface_factory();
        auto* const interface = nullptr == factory ? nullptr : static_cast<T*>(factory(name.c_str(), &status));

        if (cache && (CreateInterfaceStatus::succeeded == status) && (interface != nullptr)) {
            interfaces_.emplace(name, interface);
        }

        return interface;
    }

    template <typename T>
    T* ModuleWrapper::get_interface_or_abort(const std::string& name, const bool cache)
    {
        if (auto* const interface = get_interface<T>(name, cache); interface != nullptr) {
            return interface;
        }

        util::log_critical("Failed to retrieve \"{}\" interface.", name);
        std::abort();
    }
}
