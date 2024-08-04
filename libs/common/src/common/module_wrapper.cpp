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

#include "common/module_wrapper.hpp"
#include "util/system.hpp"
#include <cassert>

namespace Common {
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

        if (handle_ = Util::load_module(name_); is_loaded()) {
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
        const auto reason = Util::get_last_error_string();
#else
        const auto reason = Util::get_dlerror();
#endif
        Util::log_critical("Failed to load {}: {}", name_, reason);
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
