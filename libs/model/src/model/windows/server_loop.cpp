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

#include "model/server_loop.hpp"
#include "common/engine/engine_wrapper.hpp"
#include "common/engine/interface/dedicated_serverapi_interface.hpp"
#include "util/logger.hpp"
#include "util/windows/system/ntdll.hpp"
#include <chrono>
#include <cstdlib>
#include <optional>
#include <thread>

namespace {
    using SleepFunc = void (*)();
    SleepFunc sleep_thread = nullptr;

    using SleepNetFunc = int (*)();
    SleepNetFunc sleep_thread_net = nullptr;

    SleepNetFunc get_sleep_net_func() noexcept
    {
        const auto& engine = Common::EngineWrapper::get_instance();
        return engine.get_symbol_address<SleepNetFunc>("NET_Sleep_Timeout");
    }

    /// pingboost 0
    void sleep_1ms() noexcept
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }

    /// pingboost 3
    void sleep_net() noexcept
    {
        if (nullptr == sleep_thread_net) {
            sleep_1ms();
        }
        else {
            sleep_thread_net();
        }
    }

    /// pingboost 4
    void sleep_timer() noexcept
    {
        ::LARGE_INTEGER interval;
        interval.QuadPart = -1LL;
        Util::ntdll::delay_execution(FALSE, &interval);
    }

    /// pingboost 5
    void yield_thread() noexcept
    {
        std::this_thread::yield();
    }

    SleepFunc get_sleep_func(const int pingboost) noexcept
    {
        constexpr auto pingboost_net = 3;
        constexpr auto pingboost_sleep = 4;
        constexpr auto pingboost_yield = 5;

        if (pingboost_net == pingboost) {
            sleep_thread_net = get_sleep_net_func();
        }
        else if (pingboost_sleep == pingboost) {
            Util::ntdll::set_timer_resolution_or_abort(1);
        }

        switch (pingboost) {
            case pingboost_net: return sleep_net;
            case pingboost_sleep: return sleep_timer;
            case pingboost_yield: return yield_thread;
            default: return sleep_1ms;
        }
    }
}

namespace Model {
    void ServerLoop::run(Common::DedicatedServerApiInterface& serverapi_interface) noexcept
    {
        if (PingBoostLevel::target_fps == pingboost_level_ && target_fps_ > 0) {
            run(serverapi_interface, target_fps_);
        }
        else {
            const auto pingboost = static_cast<int>(pingboost_level_);
            sleep_thread = get_sleep_func(pingboost);

            while (serverapi_interface.run_frame()) {
                sleep_thread();
                process_tasks();
                process_input(serverapi_interface);
                update_status(serverapi_interface);
            }
        }
    }
}
