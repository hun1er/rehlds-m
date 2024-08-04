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

#include "util/lifecycle.hpp"
#include "util/logger.hpp"
#include <cassert>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <utility>

namespace Util::policies {
    /**
     * @brief A creation policy for \ref Util::singleton::SingletonHolder that creates a unique instance of T.
     *
     * Creates an instance of T using \c std::make_unique and stores it in a \c std::unique_ptr.
     * The instance can be destroyed by calling the destroy function, which resets the std::unique_ptr.
     *
     * @tparam T The type of the singleton instance.
     */
    template <typename T>
    class CreateUnique {
      public:
        /** @brief The type of the singleton instance. */
        using InstanceType = std::unique_ptr<T>;

        /**
         * @brief Creates an instance of T using \c std::make_unique
         *
         * @tparam Args The types of the arguments to pass to the constructor of T.
         *
         * @param args The arguments to pass to the constructor of T.
         *
         * @return A \c std::unique_ptr to the created instance of T.
         */
        template <typename... Args>
        [[nodiscard]] static InstanceType create(Args&&... args)
        {
            return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); //-V824
        }

        /**
         * @brief Destroys the instance of T by resetting the given \c std::unique_ptr.
         *
         * @param instance A reference to the \c std::unique_ptr that holds the instance of T.
         */
        static void destroy(InstanceType& instance) noexcept
        {
            instance.reset();
        }
    };

    /**
     * @brief A lifetime policy for \ref Util::singleton::SingletonHolder that schedules destruction at exit.
     *
     * Schedules the destruction of the singleton instance at program exit
     * using the \ref Util::lifecycle::at_exit function.
     *
     * @tparam T The type of the singleton instance.
     */
    template <typename T>
    class DefaultLifetime {
      public:
        /**
         * @brief Schedules the destruction of the singleton instance at program exit.
         *
         * @tparam FuncDestroy The type of the destroy function.
         *
         * @param destroy The destroy function to call at program exit.
         */
        template <typename FuncDestroy>
        static void schedule_destruction(T& /* instance */, FuncDestroy&& destroy)
        {
            at_exit(std::forward<FuncDestroy>(destroy));
        }

        /**
         * @brief Handles a dead reference to the singleton instance.
         *
         * Logs a critical error and aborts the program.
         */
        static void handle_dead_reference()
        {
            log_critical("Dead reference detected.");
            std::abort();
        }
    };

    /**
     * @brief Lifetime policy for \ref Util::singleton::SingletonHolder that does not destroy the instance.
     *
     * No actions are performed when the destruction of the singleton instance
     * is scheduled or when a dead reference is detected.
     *
     * @tparam T The type of the singleton instance.
     */
    template <typename T>
    class NoDestroy {
      public:
        /**
         * @brief Does not schedule the destruction of the singleton instance.
         *
         * No actions are performed.
         *
         * @tparam FuncDestroy The type of the destroy function.
         */
        template <typename FuncDestroy>
        static void schedule_destruction(T& /* instance */, FuncDestroy&& /* destroy */) noexcept
        {
            // No action is taken to destroy the instance.
        }

        /**
         * @brief Does not handle a dead reference to the singleton instance.
         *
         * No actions are performed.
         */
        static void handle_dead_reference() noexcept
        {
            // No action is taken to handle a dead reference.
        }
    };
}

namespace Util {
    /**
     * @brief Implements the Singleton design pattern for a type \c T.
     *
     * Provides a way to create and manage a singleton instance of type \c T,
     * using the specified creation and lifetime policies.
     *
     * @tparam T The type of the singleton instance.
     * @tparam CreationPolicy The policy used to create and destroy the singleton instance.
     * @tparam LifetimePolicy The policy used to manage the lifetime of the singleton instance.
     * @tparam ThreadSafe Whether the singleton instance should be thread-safe.
     */
    template <
      typename T, template <typename> typename CreationPolicy = policies::CreateUnique,
      template <typename> typename LifetimePolicy = policies::DefaultLifetime, bool ThreadSafe = false>
    class SingletonHolder;

    /**
     * @brief Non-thread-safe version of \ref Util::singleton::SingletonHolder
     */
    template <typename T, template <typename> typename CreationPolicy, template <typename> typename LifetimePolicy>
    class SingletonHolder<T, CreationPolicy, LifetimePolicy, false> {
      public:
        /**
         * @brief Returns a reference to the singleton instance.
         *
         * Creates the singleton instance if it does not exist yet and returns a reference to it.
         *
         * @tparam Args The types of the arguments to pass to the constructor of T.
         *
         * @param args The arguments to pass to the constructor of T.
         *
         * @return A reference to the singleton instance.
         */
        template <typename... Args>
        [[nodiscard]] static T& get_instance(Args&&... args)
        {
            if (nullptr == instance) {
                if (destroyed) {
                    LifetimePolicy<T>::handle_dead_reference();
                    destroyed = false;
                }

                instance = CreationPolicy<T>::create(std::forward<Args>(args)...);
                LifetimePolicy<T>::schedule_destruction(*instance, &destroy_singleton);
            }

            return *instance;
        }

      private:
        /// The singleton instance of type \c T.
        static inline typename CreationPolicy<T>::InstanceType instance{};

        /// Whether the singleton instance has been destroyed.
        static inline bool destroyed{};

        /// Private default constructor to prevent instantiation of this class.
        SingletonHolder() = default;

        /**
         * @brief Destroys the singleton instance.
         *
         * Destroys the singleton instance by calling the destroy function of the creation policy.
         */
        static void destroy_singleton()
        {
            assert(!destroyed);
            CreationPolicy<T>::destroy(instance);
            destroyed = true;
        }
    };

    /**
     * @brief A thread-safe version of \ref Util::singleton::SingletonHolder
     */
    template <typename T, template <typename> typename CreationPolicy, template <typename> typename LifetimePolicy>
    class SingletonHolder<T, CreationPolicy, LifetimePolicy, true> {
      public:
        /**
         * @brief Returns a reference to the singleton instance.
         *
         * Creates the singleton instance if it does not exist yet and returns a reference to it.
         *
         * @tparam Args The types of the arguments to pass to the constructor of T.
         *
         * @param args The arguments to pass to the constructor of T.
         *
         * @return A reference to the singleton instance.
         */
        template <typename... Args>
        [[nodiscard]] static T& get_instance(Args&&... args)
        {
            std::call_once(get_instance_flag, [&args...]() {
                if (destroyed) {
                    LifetimePolicy<T>::handle_dead_reference();
                    destroyed = false;
                }

                instance = CreationPolicy<T>::create(std::forward<Args>(args)...);
                LifetimePolicy<T>::schedule_destruction(*instance, &destroy_singleton);
            });

            return *instance;
        }

      private:
        /// The singleton instance of type \c T.
        static inline typename CreationPolicy<T>::InstanceType instance{};

        /// Flag used to ensure thread-safety when creating the singleton instance.
        static inline std::once_flag get_instance_flag{};

        /// Mutex used to ensure thread-safety when destroying the singleton instance.
        static inline std::mutex destroy_mutex{};

        /// Whether the singleton instance has been destroyed.
        static inline bool destroyed{};

        /// Private default constructor to prevent instantiation of this class.
        SingletonHolder() = default;

        /**
         * @brief Destroys the singleton instance in a thread-safe manner.
         *
         * Destroys the singleton instance by calling the destroy function of the creation policy in a thread-safe
         * manner.
         */
        static void destroy_singleton()
        {
            const std::lock_guard lock{destroy_mutex};
            assert(!destroyed);
            CreationPolicy<T>::destroy(instance);
            destroyed = true;
        }
    };
}
