/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "common/module_wrapper.hpp"
#include "util/system.hpp"
#include <cassert>

namespace common
{
    ModuleWrapper::ModuleWrapper(std::string name) : name_(std::move(name))
    {
        assert(!name_.empty());
    }

    ModuleWrapper::~ModuleWrapper()
    {
        unload();
    }

    bool ModuleWrapper::load()
    {
        if (is_loaded()) {
            return true;
        }

        if (handle_ = util::load_module(name_); is_loaded()) {
            interfaces_.clear();
            return true;
        }

        return false;
    }

    void ModuleWrapper::load_or_abort()
    {
        if (load()) {
            return;
        }

#ifdef _WIN32
        const auto reason = util::get_last_error_string();
#else
        const auto reason = util::get_dlerror();
#endif
        util::log_critical("Failed to load {}: {}", name_, reason);
        std::abort();
    }

    void ModuleWrapper::unload() noexcept
    {
        if (is_loaded() && unload_module(handle_)) {
            interfaces_.clear();
            handle_ = nullptr;
        }
    }
}
