/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace util
{
    /**
     * @brief A fixed-size circular buffer.
     *
     * @tparam T The type of elements in the buffer.
     * @tparam N The maximum number of elements in the buffer.
     */
    template <typename T, std::size_t N>
    class CircularBuffer
    {
      public:
        /**
         * @brief Type alias for a const reference to an element in the buffer.
         */
        using ConstReference = typename std::array<T, N>::const_reference;

        /**
         * @brief Type alias for a reference to an element in the buffer.
         */
        using Reference = typename std::array<T, N>::reference;

        /**
         * @brief Type alias for the size type of the buffer.
         */
        using SizeType = typename std::array<T, N>::size_type;

        /**
         * @brief Type alias for the type of elements in the buffer.
         */
        using ValueType = typename std::array<T, N>::value_type;

        /**
         * @brief Inserts an element at the front of the buffer.
         *
         * @param item The element to insert at the front of the buffer.
         */
        void push_front(const ValueType& item) noexcept;

        /**
         * @brief Constructs an element in place at the front of the buffer.
         *
         * @tparam Args The types of arguments to forward to the constructor of the element.
         *
         * @param args The arguments to forward to the constructor of the element.
         *
         * @return A reference to the newly constructed element at the front of the buffer.
         */
        template <typename... Args>
        Reference emplace_front(Args&&... args) noexcept;

        /**
         * @brief Inserts an element at the back of the buffer.
         *
         * @param item The element to insert at the back of the buffer.
         */
        void push_back(const ValueType& item) noexcept;

        /**
         * @brief Constructs an element in place at the back of the buffer.
         *
         * @tparam Args The types of arguments to forward to the constructor of the element.
         *
         * @param args The arguments to forward to the constructor of the element.
         *
         * @return A reference to the newly constructed element at the back of the buffer.
         */
        template <typename... Args>
        Reference emplace_back(Args&&... args) noexcept;

        /**
         * @brief Removes an element from the front of the buffer.
         */
        void pop_front() noexcept;

        /**
         * @brief Removes an element from the back of the buffer.
         */
        void pop_back() noexcept;

        /**
         * @brief Returns a reference to the element at the front of the buffer.
         *
         * @return A reference to the element at the front of the buffer.
         */
        [[nodiscard]] Reference front() noexcept;

        /**
         * @brief Returns a const reference to the element at the front of the buffer.
         *
         * @return A const reference to the element at the front of the buffer.
         */
        [[nodiscard]] ConstReference front() const noexcept;

        /**
         * @brief Returns a reference to the element at the back of the buffer.
         *
         * @return A reference to the element at the back of the buffer.
         */
        [[nodiscard]] Reference back() noexcept;

        /**
         * @brief Returns a const reference to the element at the back of the buffer.
         *
         * @return A const reference to the element at the back of the buffer.
         */
        [[nodiscard]] ConstReference back() const noexcept;

        /**
         * @brief Checks if the buffer is full.
         *
         * @return \c true if the buffer is full, \c false otherwise.
         */
        [[nodiscard]] bool full() const noexcept;

        /**
         * @brief Checks if the buffer is empty.
         *
         * @return \c true if the buffer is empty, \c false otherwise.
         */
        [[nodiscard]] bool empty() const noexcept;

        /**
         * @brief Returns the maximum number of elements that can be held in the buffer.
         *
         * @return The maximum number of elements that can be held in the buffer.
         */
        [[nodiscard]] SizeType max_size() const noexcept;

        /**
         * @brief Returns the number of elements currently held in the buffer.
         *
         * @return The number of elements currently held in the buffer.
         */
        [[nodiscard]] SizeType size() const noexcept;

        /**
         * @brief Removes all elements from the buffer.
         */
        void clear() noexcept;

        /**
         * @brief Returns a reference to the element at the specified index in the buffer.
         *
         * @param index The index of the element to return.
         *
         * @return A reference to the element at the specified index in the buffer.
         */
        [[nodiscard]] Reference operator[](SizeType index) noexcept;

        /**
         * @brief Returns a const reference to the element at the specified index in the buffer.
         *
         * @param index The index of the element to return.
         *
         * @return A const reference to the element at the specified index in the buffer.
         */
        [[nodiscard]] ConstReference operator[](SizeType index) const noexcept;

      private:
        /// The underlying fixed-size array storage.
        std::array<T, N> buffer_{};

        /// The index of the first element in the buffer.
        SizeType head_{};

        /// The index of the last element in the buffer.
        SizeType tail_{};

        /// A flag indicating whether the buffer is full.
        bool full_{};
    };

    template <typename T, std::size_t N>
    void CircularBuffer<T, N>::push_front(const ValueType& item) noexcept
    {
        tail_ = ((tail_ + N) - 1) % N;
        buffer_[tail_] = item;

        if (full_) {
            head_ = ((head_ + N) - 1) % N;
        }

        full_ = head_ == tail_;
    }

    template <typename T, std::size_t N>
    template <typename... Args>
    auto CircularBuffer<T, N>::emplace_front(Args&&... args) noexcept -> Reference
    {
        tail_ = ((tail_ + N) - 1) % N;
        buffer_[tail_] = ValueType(std::forward<Args>(args)...);

        if (full_) {
            head_ = ((head_ + N) - 1) % N;
        }

        full_ = head_ == tail_;

        return front();
    }

    template <typename T, std::size_t N>
    void CircularBuffer<T, N>::push_back(const ValueType& item) noexcept
    {
        buffer_[head_] = item;
        head_ = (head_ + 1) % N;

        if (full_) {
            tail_ = (tail_ + 1) % N;
        }

        full_ = head_ == tail_;
    }

    template <typename T, std::size_t N>
    template <typename... Args>
    auto CircularBuffer<T, N>::emplace_back(Args&&... args) noexcept -> Reference
    {
        buffer_[head_] = ValueType(std::forward<Args>(args)...);
        head_ = (head_ + 1) % N;

        if (full_) {
            tail_ = (tail_ + 1) % N;
        }

        full_ = head_ == tail_;

        return back();
    }

    template <typename T, std::size_t N>
    void CircularBuffer<T, N>::pop_front() noexcept
    {
        if (!empty()) {
            full_ = false;
            tail_ = (tail_ + 1) % N;
        }
    }

    template <typename T, std::size_t N>
    void CircularBuffer<T, N>::pop_back() noexcept
    {
        if (!empty()) {
            full_ = false;
            head_ = ((head_ + N) - 1) % N;
        }
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::front() noexcept -> Reference
    {
        return buffer_[tail_];
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::front() const noexcept -> ConstReference
    {
        return buffer_[tail_];
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::back() noexcept -> Reference
    {
        return buffer_[((head_ + N) - 1) % N];
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::back() const noexcept -> ConstReference
    {
        return buffer_[((head_ + N) - 1) % N];
    }

    template <typename T, std::size_t N>
    bool CircularBuffer<T, N>::full() const noexcept
    {
        return full_;
    }

    template <typename T, std::size_t N>
    bool CircularBuffer<T, N>::empty() const noexcept
    {
        return ((!full_) && (head_ == tail_));
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::max_size() const noexcept -> SizeType
    {
        return buffer_.max_size();
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::size() const noexcept -> SizeType
    {
        if (full_) {
            return N;
        }

        if (head_ >= tail_) {
            return head_ - tail_;
        }

        return (N + head_) - tail_;
    }

    template <typename T, std::size_t N>
    void CircularBuffer<T, N>::clear() noexcept
    {
        head_ = tail_;
        full_ = false;
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::operator[](const SizeType index) noexcept -> Reference
    {
        return buffer_[(tail_ + index) % N];
    }

    template <typename T, std::size_t N>
    auto CircularBuffer<T, N>::operator[](const SizeType index) const noexcept -> ConstReference
    {
        return buffer_[(tail_ + index) % N];
    }
}
