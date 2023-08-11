/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/server_loop.hpp"
#include "common/engine/engine_wrapper.hpp"
#include "common/engine/interface/dedicated_serverapi_interface.hpp"
#include "model/cmdline_args.hpp"
#include "util/logger.hpp"
#include "util/windows/system/ntdll.hpp"
#include <chrono>
#include <cstdlib>
#include <optional>
#include <thread>

namespace
{
    using SleepFunc = void (*)();
    SleepFunc sleep_thread = nullptr;

    using SleepNetFunc = int (*)();
    SleepNetFunc sleep_thread_net = nullptr;

    SleepNetFunc get_sleep_net_func() noexcept
    {
        const auto& engine = common::EngineWrapper::get_instance();
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
        util::ntdll::delay_execution(FALSE, &interval);
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
            util::ntdll::set_timer_resolution_or_abort(1);
        }

        switch (pingboost) {
            case pingboost_net: return sleep_net;
            case pingboost_sleep: return sleep_timer;
            case pingboost_yield: return yield_thread;
            default: return sleep_1ms;
        }
    }
}

namespace model
{
    void ServerLoop::run(common::DedicatedServerApiInterface& serverapi_interface) noexcept
    {
        if (const auto target_fps = cmdline_args_->get_param_option_as<int>("targetfps").value_or(0); target_fps > 0) {
            run(serverapi_interface, target_fps);
        }
        else {
            const auto pingboost = cmdline_args_->get_param_option_as<int>("pingboost").value_or(0);
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
