/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/lifecycle.hpp"
#include <cstdlib>
#include <memory>
#include <mutex>
#include <stack>
#include <utility>

namespace
{
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

namespace util
{
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
