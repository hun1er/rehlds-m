/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace util
{
    /**
     * @brief A thread-safe implementation of a queue.
     *
     * Provides a thread-safe implementation of a queue using a mutex and condition variable.
     * Supports all the operations of a \c std::queue, but with added thread-safety.
     *
     * @tparam T The type of elements in the queue.
     */
    template <typename T>
    class ThreadSafeQueue
    {
      public:
        /**
         * @brief Type alias for the underlying container type.
         */
        using ContainerType = typename std::queue<T>::container_type;

        /**
         * @brief Type alias for a const reference to an element in the queue.
         */
        using ConstReference = typename std::queue<T>::const_reference;

        /**
         * @brief Type alias for a reference to an element in the queue.
         */
        using Reference = typename std::queue<T>::reference;

        /**
         * @brief Type alias for the size type of the queue.
         */
        using SizeType = typename std::queue<T>::size_type;

        /**
         * @brief Type alias for the type of elements in the queue.
         */
        using ValueType = typename std::queue<T>::value_type;

        /**
         * @brief Constructs a new ThreadSafeQueue object with the given container.
         *
         * @param container The container to use as the underlying container for the queue.
         */
        explicit ThreadSafeQueue(const ContainerType& container);

        /**
         * @brief Constructs a new ThreadSafeQueue object with the given container (rvalue).
         *
         * @param container The container to use as the underlying container for the queue (rvalue).
         */
        explicit ThreadSafeQueue(ContainerType&& container = ContainerType());

        /**
         * @brief Copy constructor.
         *
         * Constructs a new ThreadSafeQueue object that is a copy of the given ThreadSafeQueue object.
         *
         * @param other The ThreadSafeQueue object to copy.
         */
        ThreadSafeQueue(const ThreadSafeQueue& other);

        /**
         * @brief Move constructor.
         *
         * Constructs a new ThreadSafeQueue object by moving the contents of the given ThreadSafeQueue object.
         *
         * @param other The ThreadSafeQueue object to move (rvalue).
         */
        ThreadSafeQueue(ThreadSafeQueue&& other) noexcept;

        /**
         * @brief Constructs a new ThreadSafeQueue object with the given allocator.
         *
         * @param alloc The allocator to use for the underlying container.
         */
        template <class Alloc>
        explicit ThreadSafeQueue(const Alloc& alloc);

        /**
         * @brief Constructs a new ThreadSafeQueue object with the given container and allocator.
         *
         * @param container The container to use as the underlying container for the queue.
         * @param alloc The allocator to use for the underlying container.
         */
        template <class Alloc>
        ThreadSafeQueue(const ContainerType& container, const Alloc& alloc);

        /**
         * @brief Constructs a new ThreadSafeQueue object with the given container (rvalue) and allocator.
         *
         * @param container The container to use as the underlying container for the queue (rvalue).
         * @param alloc The allocator to use for the underlying container.
         */
        template <class Alloc>
        ThreadSafeQueue(ContainerType&& container, const Alloc& alloc);

        /**
         * @brief Copy constructor with allocator.
         *
         * Constructs a new ThreadSafeQueue object that is a copy of the given ThreadSafeQueue object,
         * using the given allocator.
         *
         * @param other The ThreadSafeQueue object to copy.
         * @param alloc The allocator to use for the underlying container.
         */
        template <class Alloc>
        ThreadSafeQueue(const ThreadSafeQueue& other, const Alloc& alloc);

        /**
         * @brief Move constructor with allocator.
         *
         * Constructs a new ThreadSafeQueue object by moving the contents of the given ThreadSafeQueue object,
         * using the given allocator.
         *
         * @param other The ThreadSafeQueue object to move (rvalue).
         * @param alloc The allocator to use for the underlying container.
         */
        template <class Alloc>
        ThreadSafeQueue(ThreadSafeQueue&& other, const Alloc& alloc) noexcept;

        /**
         * @brief Destructor.
         *
         * Destructs the ThreadSafeQueue object.
         */
        ~ThreadSafeQueue() = default;

        /**
         * @brief Deleted copy assignment operator.
         *
         * Copy assignment is not allowed for ThreadSafeQueue objects.
         */
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

        /**
         * @brief Deleted move assignment operator.
         *
         * Move assignment is not allowed for ThreadSafeQueue objects.
         */
        ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

        /**
         * @brief Pushes a new element to the back of the queue.
         *
         * Adds a new element to the back of the queue.
         * The element is constructed by copying the given value.
         *
         * @param item The value of the element to push to the back of the queue.
         */
        void push(const T& item);

        /**
         * @brief Pushes a new element to the back of the queue.
         *
         * Adds a new element to the back of the queue.
         * The element is constructed by moving the given value (rvalue).
         *
         * @param item The value of the element to push to the back of the queue (rvalue).
         */
        void push(T&& item);

        /**
         * @brief Constructs a new element in-place at the back of the queue.
         *
         * Adds a new element to the back of the queue.
         * The element is constructed in-place with the given arguments.
         *
         * @tparam Args The types of arguments to forward to the constructor of the element.
         * @param args The arguments to forward to the constructor of the element.
         *
         * @return A reference to the newly constructed element at the back of the queue.
         */
        template <typename... Args>
        decltype(auto) emplace(Args&&... args);

        /**
         * @brief Removes and returns an element from the front of the queue.
         *
         * Removes and returns an element from the front of the queue.
         * If the queue is empty, this method blocks until an element becomes available.
         *
         * @return The removed element from the front of the queue.
         */
        T pop();

        /**
         * @brief Clears the queue.
         *
         * Removes all elements from the queue, leaving it with a size of 0.
         */
        void clear();

        /**
         * @brief Returns a reference to the element at the front of the queue.
         *
         * Returns a reference to the element at the front of the queue.
         * If the queue is empty, this method blocks until an element becomes available.
         *
         * @return A reference to the element at the front of the queue.
         */
        [[nodiscard]] Reference front();

        /**
         * @brief Returns a const reference to the element at the front of the queue.
         *
         * Returns a const reference to the element at the front of the queue.
         * If the queue is empty, this method blocks until an element becomes available.
         *
         * @return A const reference to the element at the front of the queue.
         */
        [[nodiscard]] ConstReference front() const;

        /**
         * @brief Returns a reference to the element at the back of the queue.
         *
         * Returns a reference to the element at the back of the queue.
         * If the queue is empty, this method blocks until an element becomes available.
         *
         * @return A reference to the element at the back of the queue.
         */
        [[nodiscard]] Reference back();

        /**
         * @brief Returns a const reference to the element at the back of the queue.
         *
         * Returns a const reference to the element at the back of the queue.
         * If the queue is empty, this method blocks until an element becomes available.
         *
         * @return A const reference to the element at the back of the queue.
         */
        [[nodiscard]] ConstReference back() const;

        /**
         * @brief Returns the number of elements in the queue.
         *
         * Returns the number of elements in the queue.
         *
         * @return The number of elements in the queue.
         */
        [[nodiscard]] SizeType size() const;

        /**
         * @brief Checks if the queue is empty.
         *
         * Returns \c true if the queue is empty, \c false otherwise.
         *
         * @return \c true if the queue is empty, \c false otherwise.
         */
        [[nodiscard]] bool empty() const;

      private:
        /// Mutex to protect access to the queue.
        mutable std::mutex mutex_{};

        /// Condition variable to wait for elements in the queue.
        std::condition_variable cond_var_{};

        /// The underlying queue of elements.
        std::queue<T> queue_{};
    };

    template <typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(const ContainerType& container) : queue_(container)
    {
    }

    template <typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(ContainerType&& container) : queue_(std::move(container))
    {
    }

    template <typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(const ThreadSafeQueue& other)
    {
        const std::scoped_lock lock{other.mutex_};
        queue_ = other.queue_; // cppcheck-suppress useInitializationList
    }

    template <typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue&& other) noexcept
    {
        const std::scoped_lock lock{other.mutex_};
        queue_ = std::move(other.queue_); // cppcheck-suppress useInitializationList
    }

    template <typename T>
    template <class Alloc>
    ThreadSafeQueue<T>::ThreadSafeQueue(const Alloc& alloc) : queue_(alloc)
    {
    }

    template <typename T>
    template <class Alloc>
    ThreadSafeQueue<T>::ThreadSafeQueue(const ContainerType& container, const Alloc& alloc) : queue_(container, alloc)
    {
    }

    template <typename T>
    template <class Alloc>
    ThreadSafeQueue<T>::ThreadSafeQueue(ContainerType&& container, const Alloc& alloc) :
      queue_(std::move(container), alloc)
    {
    }

    template <typename T>
    template <class Alloc>
    ThreadSafeQueue<T>::ThreadSafeQueue(const ThreadSafeQueue& other, const Alloc& alloc)
    {
        const std::scoped_lock lock{other.mutex_};
        queue_ = std::queue<T, ContainerType>(other.queue_, alloc); // cppcheck-suppress useInitializationList
    }

    template <typename T>
    template <class Alloc>
    ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue&& other, const Alloc& alloc) noexcept
    {
        const std::scoped_lock lock{other.mutex_};

        // cppcheck-suppress useInitializationList
        queue_ = std::queue<T, ContainerType>(std::move(other.queue_), alloc);
    }

    template <typename T>
    void ThreadSafeQueue<T>::push(const T& item)
    {
        const std::scoped_lock lock{mutex_};
        queue_.push(item);
    }

    template <typename T>
    void ThreadSafeQueue<T>::push(T&& item)
    {
        const std::scoped_lock lock{mutex_};
        queue_.push(std::move(item));
    }

    template <typename T>
    template <typename... Args>
    decltype(auto) ThreadSafeQueue<T>::emplace(Args&&... args)
    {
        const std::scoped_lock lock{mutex_};
        return queue_.emplace(std::forward<Args>(args)...);
    }

    template <typename T>
    T ThreadSafeQueue<T>::pop()
    {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this]() {
            return !queue_.empty();
        });

        auto element = queue_.front();
        queue_.pop();

        return element;
    }

    template <typename T>
    void ThreadSafeQueue<T>::clear()
    {
        const std::scoped_lock lock{mutex_};

        std::queue<T> empty_queue{};
        queue_.swap(empty_queue);
    }

    template <typename T>
    typename ThreadSafeQueue<T>::Reference ThreadSafeQueue<T>::front()
    {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this]() {
            return !queue_.empty();
        });

        return queue_.front();
    }

    template <typename T>
    typename ThreadSafeQueue<T>::ConstReference ThreadSafeQueue<T>::front() const
    {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this]() {
            return !queue_.empty();
        });

        return queue_.front();
    }

    template <typename T>
    typename ThreadSafeQueue<T>::Reference ThreadSafeQueue<T>::back()
    {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this]() {
            return !queue_.empty();
        });

        return queue_.back();
    }

    template <typename T>
    typename ThreadSafeQueue<T>::ConstReference ThreadSafeQueue<T>::back() const
    {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this]() {
            return !queue_.empty();
        });

        return queue_.back();
    }

    template <typename T>
    typename ThreadSafeQueue<T>::SizeType ThreadSafeQueue<T>::size() const
    {
        const std::scoped_lock lock{mutex_};
        return queue_.size();
    }

    template <typename T>
    bool ThreadSafeQueue<T>::empty() const
    {
        const std::scoped_lock lock{mutex_};
        return queue_.empty();
    }
}
