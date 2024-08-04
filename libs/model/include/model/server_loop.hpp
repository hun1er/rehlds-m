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

#pragma once

#include "common/platform.hpp"
#include "model/server_status.hpp"
#include "util/observable.hpp"
#include "util/threadsafe_queue.hpp"
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace Common {
    class DedicatedServerApiInterface;
}

namespace Model {
    enum class ServerLoopEvent { status_updated };
    enum class PingBoostLevel { not_set, level1, level2, level3, level4, level5, target_fps };

    /**
     * @brief Runs the main loop of a server application.
     */
    class ServerLoop final : public Util::Observable<ServerLoopEvent> {
      public:
        /**
         * @brief A type alias for a thread-safe queue of \c std::function<void()>
         */
        using TaskQueue = Util::ThreadSafeQueue<std::function<void()>>;

        /**
         * @brief A type alias for a thread-safe queue of \c std::string
         */
        using InputQueue = Util::ThreadSafeQueue<std::string>;

        [[nodiscard]] PingBoostLevel get_pingboost_level() const noexcept;

        void set_pingboost_level(PingBoostLevel level) noexcept;

        [[nodiscard]] int get_target_fps() const noexcept;

        void set_target_fps(int fps) noexcept;

        /**
         * @brief Runs the server loop at the specified target FPS.
         *
         * Processes tasks and input, and updates the server status.
         *
         * @param serverapi_interface The server API interface to use.
         * @param target_fps The target FPS to run at.
         */
        void run(Common::DedicatedServerApiInterface& serverapi_interface, int target_fps) noexcept;

        /**
         * @brief Runs the server loop.
         *
         * Processes tasks and input, and updates the server status.
         *
         * @param serverapi_interface The server API interface to use.
         */
        void run(Common::DedicatedServerApiInterface& serverapi_interface) noexcept;

        /**
         * @brief Gets the current status of the server.
         *
         * @return const ServerStatus& The current status of the server.
         */
        [[nodiscard]] const ServerStatus& get_status() const noexcept;

        /**
         * @brief Enqueues an input string to be processed by the server loop.
         *
         * @param input The input string to enqueue.
         */
        void enqueue_input(std::string input);

        /**
         * @brief Enqueues an input string view to be processed by the server loop.
         *
         * @param input The input string to enqueue.
         */
        void enqueue_input(std::string_view input);

        /**
         * @brief Enqueues a task to be executed by the server loop
         * and returns a future for the result of the task.
         *
         * @tparam Result The result type of the task.
         * @param task The task to enqueue.
         *
         * @return A future for the result of the task.
         */
        template <typename Result = void>
        std::future<Result> enqueue_task(std::function<Result()> task);

      private:
        /// Processes task from the task queue.
        ATTR_HOT void process_tasks();

        /// Processes input from the input queue.
        ATTR_HOT void process_input(Common::DedicatedServerApiInterface& serverapi_interface);

        /// Updates the server status.
        ATTR_HOT void update_status(Common::DedicatedServerApiInterface& serverapi_interface);

        PingBoostLevel pingboost_level_{};

        int target_fps_{};

        /// The current status of the server.
        ServerStatus status_{};

        /// The task queue for tasks to be executed by the server loop.
        TaskQueue task_queue_{};

        /// The input queue for input to be processed by the server loop.
        InputQueue input_queue_{};
    };

    inline PingBoostLevel ServerLoop::get_pingboost_level() const noexcept
    {
        return pingboost_level_;
    }

    inline void ServerLoop::set_pingboost_level(const PingBoostLevel level) noexcept
    {
        pingboost_level_ = level;
    }

    inline int ServerLoop::get_target_fps() const noexcept
    {
        return target_fps_;
    }

    inline void ServerLoop::set_target_fps(const int fps) noexcept
    {
        target_fps_ = fps;
    }

    inline const ServerStatus& ServerLoop::get_status() const noexcept
    {
        return status_;
    }

    inline void ServerLoop::enqueue_input(std::string input)
    {
        input.push_back('\n');
        input_queue_.emplace(std::move(input));
    }

    inline void ServerLoop::enqueue_input(const std::string_view input)
    {
        enqueue_input(std::string{input});
    }

    template <typename Result>
    std::future<Result> ServerLoop::enqueue_task(std::function<Result()> task)
    {
        using TaskType = std::packaged_task<Result()>;
        const auto packaged_task = std::make_shared<TaskType>(std::move(task));

        task_queue_.emplace([packaged_task] {
            (*packaged_task)();
        });

        return packaged_task->get_future();
    }
}
