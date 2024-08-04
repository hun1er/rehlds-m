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

#include "common/filesystem/interface/filesystem_interface.hpp"
#include "common/module_wrapper.hpp"
#include "util/singleton.hpp"

namespace Common {
    /**
     * @brief Enumeration of available filesystem interfaces.
     */
    enum class FileSystemModuleInterface {
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
    struct FileSystemInterfaceTraits<FileSystemModuleInterface::filesystem> {
        using InterfaceType = FileSystemInterface;
    };

    /**
     * @brief A wrapper class for managing the filesystem module.
     */
    class FileSystemWrapper final : public ModuleWrapper {
        friend class Util::policies::CreateUnique<FileSystemWrapper>;

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
        return Util::SingletonHolder<FileSystemWrapper>::get_instance();
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
