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

#include "common/engine/interface/dedicated_serverapi_interface.hpp"
#include "common/engine/interface/system_interface.hpp"
#include "common/engine/interface/systemmodule_interface.hpp"
#include "common/module_wrapper.hpp"
#include "util/singleton.hpp"

namespace Common {
    /**
     * @brief Enumeration of available engine interfaces.
     */
    enum class EngineInterface {
        server_api,   ///< "VENGINE_HLDS_API_VERSION002"
        system_module ///< "basesystem002"
    };

    /**
     * @brief Traits for a given engine interface.
     *
     * @tparam Interface The engine interface to provide traits for.
     */
    template <EngineInterface Interface>
    struct EngineInterfaceTraits;

    template <>
    struct EngineInterfaceTraits<EngineInterface::server_api> {
        using InterfaceType = DedicatedServerApiInterface;
    };

    template <>
    struct EngineInterfaceTraits<EngineInterface::system_module> {
        using InterfaceType = SystemModuleInterface;
    };

    /**
     * @brief A wrapper class for managing the engine module.
     */
    class EngineWrapper final : public ModuleWrapper {
        friend class Util::policies::CreateUnique<EngineWrapper>;

      public:
#ifdef _WIN32
        /// The filename of the engine module on Windows.
        static constexpr auto* MODULE_FILENAME = "swds.dll";
#else
        /// The filename of the engine module on non-Windows platforms.
        static constexpr auto* MODULE_FILENAME = "engine_i486.so";
#endif
        /**
         * @brief Gets the singleton instance of the EngineWrapper class.
         *
         * @return A reference to the singleton instance of the EngineWrapper class.
         */
        [[nodiscard]] static EngineWrapper& get_instance();

        /**
         * @brief Retrieves an interface from the engine module using its type.
         *
         * @tparam Interface The engine interface to retrieve.
         *
         * @param cache Whether to cache the retrieved interface for future use.
         *
         * @return A pointer to the retrieved interface, or nullptr if not found or failed to retrieve.
         */
        template <EngineInterface Interface>
        [[nodiscard]] auto* get_interface(bool cache = true);

        /**
         * @brief Retrieves an interface from the engine module using its type
         * and aborts if the interface retrieval fails.
         *
         * @tparam Interface The engine interface to retrieve.
         *
         * @param cache Whether to cache the retrieved interface for future use.
         *
         * @return A pointer to the retrieved interface. If the interface retrieval fails, the program will abort.
         */
        template <EngineInterface Interface>
        [[nodiscard]] auto* get_interface_or_abort(bool cache = true);

        /**
         * @brief Gets the SystemInterface from the engine module.
         *
         * @param cache Whether to cache the retrieved interface for future use.
         *
         * @return A pointer to the retrieved SystemInterface interface, or nullptr if not found or failed to retrieve.
         */
        [[nodiscard]] SystemInterface* get_interface_system(bool cache = true);

      private:
        /**
         * @brief Constructs a new EngineWrapper object.
         */
        EngineWrapper();
    };

    inline EngineWrapper::EngineWrapper() : ModuleWrapper(MODULE_FILENAME)
    {
    }

    inline EngineWrapper& EngineWrapper::get_instance()
    {
        return Util::SingletonHolder<EngineWrapper>::get_instance();
    }

    template <EngineInterface Interface>
    auto* EngineWrapper::get_interface(const bool cache)
    {
        using InterfaceType = typename EngineInterfaceTraits<Interface>::InterfaceType;
        return ModuleWrapper::get_interface<InterfaceType>(InterfaceType::INTERFACE_NAME, cache);
    }

    template <EngineInterface Interface>
    auto* EngineWrapper::get_interface_or_abort(const bool cache)
    {
        using InterfaceType = typename EngineInterfaceTraits<Interface>::InterfaceType;
        return ModuleWrapper::get_interface_or_abort<InterfaceType>(InterfaceType::INTERFACE_NAME, cache);
    }

    inline SystemInterface* EngineWrapper::get_interface_system(const bool cache)
    {
        auto* const system_module = get_interface<EngineInterface::system_module>(cache);
        return nullptr == system_module ? nullptr : system_module->get_system();
    }
}
