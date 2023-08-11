/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/filesystem/interface/filesystem_interface.hpp"
#include "common/module_wrapper.hpp"
#include "util/singleton.hpp"

namespace common
{
    /**
     * @brief Enumeration of available filesystem interfaces.
     */
    enum class FileSystemModuleInterface
    {
        filesystem ///< "VFileSystem009"
    };

    /**
     * @brief Traits for a given filesystem interface.
     *
     * @tparam Interface The filesystem interface to provide traits for.
     */
    template <FileSystemModuleInterface Interface>
    struct FileSystemInterfaceTraits;

    template <>
    struct FileSystemInterfaceTraits<FileSystemModuleInterface::filesystem>
    {
        using InterfaceType = FileSystemInterface;
    };

    /**
     * @brief A wrapper class for managing the filesystem module.
     */
    class FileSystemWrapper final : public ModuleWrapper
    {
        friend class util::policies::CreateUnique<FileSystemWrapper>;

      public:
#ifdef _WIN32
        /// The filename of the filesystem module on Windows.
        static constexpr auto* MODULE_FILENAME = "FileSystem_Stdio.dll";
#else
        /// The filename of the filesystem module on non-Windows platforms.
        static constexpr auto* MODULE_FILENAME = "filesystem_stdio.so";
#endif
        /**
         * @brief Gets the singleton instance of the FileSystemWrapper class.
         *
         * @return A reference to the singleton instance of the FileSystemWrapper class.
         */
        [[nodiscard]] static FileSystemWrapper& get_instance();

        /**
         * @brief Retrieves an interface from the filesystem module using its type.
         *
         * @tparam Interface The filesystem interface to retrieve.
         *
         * @param cache Whether to cache the retrieved interface for future use.
         *
         * @return A pointer to the retrieved interface, or nullptr if not found or failed to retrieve.
         */
        template <FileSystemModuleInterface Interface>
        [[nodiscard]] auto* get_interface(bool cache = true);

        /**
         * @brief Retrieves an interface from the filesystem module using its type
         * and aborts if the interface retrieval fails.
         *
         * @tparam Interface The filesystem interface to retrieve.
         *
         * @param cache Whether to cache the retrieved interface for future use.
         *
         * @return A pointer to the retrieved interface. If the interface retrieval fails, the program will abort.
         */
        template <FileSystemModuleInterface Interface>
        [[nodiscard]] auto* get_interface_or_abort(bool cache = true);

      private:
        /**
         * @brief Constructs a new FileSystemWrapper object.
         */
        FileSystemWrapper();
    };

    inline FileSystemWrapper::FileSystemWrapper() : ModuleWrapper(MODULE_FILENAME)
    {
    }

    inline FileSystemWrapper& FileSystemWrapper::get_instance()
    {
        return util::SingletonHolder<FileSystemWrapper>::get_instance();
    }

    template <FileSystemModuleInterface Interface>
    auto* FileSystemWrapper::get_interface(const bool cache)
    {
        using InterfaceType = typename FileSystemInterfaceTraits<Interface>::InterfaceType;
        return ModuleWrapper::get_interface<InterfaceType>(InterfaceType::INTERFACE_NAME, cache);
    }

    template <FileSystemModuleInterface Interface>
    auto* FileSystemWrapper::get_interface_or_abort(const bool cache)
    {
        using InterfaceType = typename FileSystemInterfaceTraits<Interface>::InterfaceType;
        return ModuleWrapper::get_interface_or_abort<InterfaceType>(InterfaceType::INTERFACE_NAME, cache);
    }
}
