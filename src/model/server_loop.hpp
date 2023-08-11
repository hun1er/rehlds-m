/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
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

namespace common
{
    class DedicatedServerApiInterface;
}

namespace model
{
    enum class ServerLoopEvent
    {
        status_updated
    };

    /**
     * @brief Runs the main loop of a server application.
     */
    class ServerLoop final : public util::Observable<ServerLoopEvent>
    {
      public:
        /**
         * @brief A type alias for a thread-safe queue of \c std::function<void()>
         */
        using TaskQueue = util::ThreadSafeQueue<std::function<void()>>;

        /**
         * @brief A type alias for a thread-safe queue of \c std::string
         */
        using InputQueue = util::ThreadSafeQueue<std::string>;

        /**
         * @brief A type alias for a shared pointer to \c model::CmdLineArgs
         */
        using CmdLineArgsPtr = std::shared_ptr<class CmdLineArgs>;

        /**
         * @brief Constructs a ServerLoop object.
         *
         * @param cmdline_args The command line arguments to use.
         */
        explicit ServerLoop(CmdLineArgsPtr cmdline_args);

        /**
         * @brief Runs the server loop at the specified target FPS.
         *
         * Processes tasks and input, and updates the server status.
         *
         * @param serverapi_interface The server API interface to use.
         * @param target_fps The target FPS to run at.
         */
        void run(common::DedicatedServerApiInterface& serverapi_interface, int target_fps) noexcept;

        /**
         * @brief Runs the server loop.
         *
         * Processes tasks and input, and updates the server status.
         *
         * @param serverapi_interface The server API interface to use.
         */
        void run(common::DedicatedServerApiInterface& serverapi_interface) noexcept;

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
        /// The hlds command line arguments.
        CmdLineArgsPtr cmdline_args_;

        /// The current status of the server.
        ServerStatus status_{};

        /// The task queue for tasks to be executed by the server loop.
        TaskQueue task_queue_{};

        /// The input queue for input to be processed by the server loop.
        InputQueue input_queue_{};

        /// Processes task from the task queue.
        ATTR_HOT void process_tasks();

        /// Processes input from the input queue.
        ATTR_HOT void process_input(common::DedicatedServerApiInterface& serverapi_interface);

        /// Updates the server status.
        ATTR_HOT void update_status(common::DedicatedServerApiInterface& serverapi_interface);
    };

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
