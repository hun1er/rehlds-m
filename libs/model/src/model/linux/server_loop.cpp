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
#include "util/signal.hpp"
#include <sys/time.h>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <optional>
#include <poll.h>
#include <thread>
#include <unistd.h>

namespace {
    using SleepFunc = void (*)();
    SleepFunc sleep_thread = nullptr;

    using SleepNetFunc = int (*)();
    SleepNetFunc sleep_thread_net = nullptr;

    /// Track pause status
    volatile std::sig_atomic_t paused = 0;

    int set_timer_real() noexcept
    {
        constexpr itimerval timer_value{
          {0, 0   },
          {0, 1000}
        };

        return ::setitimer(::ITIMER_REAL, &timer_value, nullptr);
    }

    void sigalrm_handler(const int /* signal_number */) noexcept
    {
        if (SIG_ERR == std::signal(SIGALRM, &sigalrm_handler)) {
            std::abort();
        }

        if (0 == paused) {
            set_timer_real();
        }
    }

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

    /// pingboost 1
    void sleep_timer() noexcept
    {
        paused = 0;

        if (0 == set_timer_real()) {
            ::pause();
        }

        paused = 1;
    }

    /// pingboost 2
    void sleep_1ms_poll() noexcept
    {
        ::poll(nullptr, 0, 1);
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
    void sleep_1us() noexcept
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1us);
    }

    /// pingboost 5
    void yield_thread() noexcept
    {
        std::this_thread::yield();
    }

    SleepFunc get_sleep_func(const int pingboost) noexcept
    {
        constexpr auto pingboost_timer = 1;
        constexpr auto pingboost_poll = 2;
        constexpr auto pingboost_net = 3;
        constexpr auto pingboost_sleep = 4;
        constexpr auto pingboost_yield = 5;

        if (pingboost_timer == pingboost) {
            if (!Util::try_set_signal_handler(SIGALRM, &sigalrm_handler)) {
                std::abort();
            }
        }
        else if (pingboost_net == pingboost) {
            sleep_thread_net = get_sleep_net_func();
        }

        switch (pingboost) {
            case pingboost_timer: return &sleep_timer;
            case pingboost_poll: return &sleep_1ms_poll;
            case pingboost_net: return &sleep_net;
            case pingboost_sleep: return &sleep_1us;
            case pingboost_yield: return &yield_thread;
            default: return &sleep_1ms;
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
            }
        }
    }
}
