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

#include "util/lifecycle.hpp"
#include <cstdlib>
#include <memory>
#include <mutex>
#include <stack>
#include <utility>

namespace {
    std::mutex mutex{};
    std::unique_ptr<std::stack<std::function<void()>>> callbacks{};

    void run_callbacks()
    {
        if (nullptr == callbacks) {
            return;
        }

        while (!callbacks->empty()) {
            callbacks->top()();
            callbacks->pop();
        }

        callbacks.reset(nullptr);
    }
}

namespace Util {
    bool at_exit(std::function<void()> callback) noexcept
    {
        const std::scoped_lock lock{mutex};

        if (nullptr == callbacks) {
            callbacks = std::make_unique<std::stack<std::function<void()>>>();

            if ((std::atexit(run_callbacks) != 0) || (std::at_quick_exit(run_callbacks) != 0)) {
                callbacks.reset(nullptr);
                return false;
            }
        }

        callbacks->emplace(std::move(callback));

        return true;
    }
}
