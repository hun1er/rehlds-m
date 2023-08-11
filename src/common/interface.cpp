/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "common/interface.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>

namespace
{
    // Implementation of the "Construct On First Use" idiom
    [[nodiscard]] auto& get_interface_registry_list()
    {
        static const auto interface_registry_list =
          std::make_unique<std::unordered_map<std::string, common::InterfaceRegistry::InstantiateFunc>>();

        return *interface_registry_list;
    }
}

namespace common
{
    InterfaceRegistry::InterfaceRegistry(InstantiateFunc instantiate_func, const std::string& interface_name)
    {
        assert(!interface_name.empty());
        auto& interface_registry_list = get_interface_registry_list();
        interface_registry_list[interface_name] = std::move(instantiate_func);
    }
}

common::CommonInterface* CreateInterface(const char* const name, common::CreateInterfaceStatus* const status)
{
    using namespace common;
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
