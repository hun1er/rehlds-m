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

#include "common/interface.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>

namespace {
    // Implementation of the "Construct On First Use" idiom
    [[nodiscard]] auto& get_interface_registry_list()
    {
        static const auto interface_registry_list =
          std::make_unique<std::unordered_map<std::string, Common::InterfaceRegistry::InstantiateFunc>>();

        return *interface_registry_list;
    }
}

namespace Common {
    InterfaceRegistry::InterfaceRegistry(InstantiateFunc instantiate_func, const std::string& interface_name)
    {
        assert(!interface_name.empty());
        auto& interface_registry_list = get_interface_registry_list();
        interface_registry_list[interface_name] = std::move(instantiate_func);
    }
}

Common::CommonInterface* CreateInterface(const char* const name, Common::CreateInterfaceStatus* const status)
{
    using namespace Common;
    CommonInterface* interface = nullptr;

    if ((name != nullptr) && (*name != '\0')) {
        const auto& interface_registry_list = get_interface_registry_list();

        if (const auto& it = interface_registry_list.find(name); it != interface_registry_list.end()) {
            interface = it->second();
        }
    }

    if (status != nullptr) {
        *status = nullptr == interface ? CreateInterfaceStatus::failed : CreateInterfaceStatus::succeeded;
    }

    return interface;
}
