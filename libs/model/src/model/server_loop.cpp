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
#include "common/engine/interface/dedicated_serverapi_interface.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <utility>

#ifdef _WIN32
  #include "util/windows/system/ntdll.hpp"
#else
  #include <thread>
#endif

namespace {
    /// Checks if it is time to update the status of the server.
    ATTR_HOT bool is_time_to_update() noexcept
    {
        using namespace std::chrono;

        constexpr auto update_interval = milliseconds{500};
        static auto last_update_time = high_resolution_clock::now();

        const auto current_time = high_resolution_clock::now(); // -V656
        const auto elapsed_time = duration_cast<milliseconds>(current_time - last_update_time);

        if (elapsed_time >= update_interval) {
            last_update_time = current_time;
            return true;
        }

        return false;
    }

    /// Calculates an adaptive smoothing coefficient for use in an exponential moving average calculation.
    ATTR_HOT double calculate_adaptive_smoothing_coefficient(
      const std::chrono::nanoseconds& duration, const std::chrono::nanoseconds& ama_duration) noexcept
    {
        constexpr auto fast_smoothing_coefficient = 1.0;
        constexpr auto slow_smoothing_coefficient = 0.1;

        if (0 == ama_duration.count()) {
            return fast_smoothing_coefficient;
        }

        return fast_smoothing_coefficient +
               ((slow_smoothing_coefficient - fast_smoothing_coefficient) *
                std::exp(-std::abs((duration.count() - ama_duration.count()) / ama_duration.count())));
    }
}

namespace Model {
    void ServerLoop::run(Common::DedicatedServerApiInterface& serverapi_interface, int target_fps) noexcept
    {
        assert(target_fps > 0);

        constexpr auto fps_increase_factor = 1.015;
        target_fps = static_cast<int>(target_fps * fps_increase_factor);

        const auto target_sleep_duration = std::chrono::nanoseconds(1'000'000'000 / target_fps);
        auto ama_duration = std::chrono::nanoseconds{0}; // Adaptive moving average of loop execution times

#ifdef _WIN32
        ::LARGE_INTEGER interval{};
        Util::ntdll::set_timer_resolution_or_abort(1);
#endif

        while (serverapi_interface.run_frame()) {
            const auto start_time = std::chrono::steady_clock::now();

#ifdef _WIN32
            const auto sleep_duration = std::max(std::chrono::nanoseconds{100}, target_sleep_duration - ama_duration);
            interval.QuadPart = sleep_duration.count() / (-100LL);
            Util::ntdll::delay_execution(FALSE, &interval);
            update_status(serverapi_interface);
#else
            const auto sleep_duration = std::max(std::chrono::nanoseconds{1}, target_sleep_duration - ama_duration);
            std::this_thread::sleep_for(sleep_duration);
#endif
            process_tasks();
            process_input(serverapi_interface);

            const auto end_time = std::chrono::steady_clock::now();
            const auto duration = end_time - start_time - sleep_duration;
            const auto alpha = calculate_adaptive_smoothing_coefficient(duration, ama_duration);

            ama_duration =
              std::chrono::duration_cast<std::chrono::nanoseconds>((alpha * duration) + ((1 - alpha) * ama_duration));
        }
    }

    void ServerLoop::process_tasks()
    {
        if (!task_queue_.empty()) {
            const auto task = task_queue_.pop();
            task();
        }
    }

    void ServerLoop::process_input(Common::DedicatedServerApiInterface& serverapi_interface)
    {
        if (!input_queue_.empty()) {
            const auto input = input_queue_.pop();
            serverapi_interface.add_console_text(input.c_str());
        }
    }

    void ServerLoop::update_status(Common::DedicatedServerApiInterface& serverapi_interface)
    {
        if (!is_time_to_update()) {
            return;
        }

        status_.current_map.clear();
        status_.current_map.resize(32U, '\0');
        serverapi_interface.update_status(
          &status_.fps, &status_.num_players, &status_.max_players, status_.current_map.data());

        notify(ServerLoopEvent::status_updated);
    }
}
