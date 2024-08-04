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

#include "common/object_container.hpp"
#include "common/platform.hpp"
#include <algorithm>
#include <deque>
#include <utility>

namespace Common {
    /**
     * @brief A container for storing objects.
     */
    class ObjectList final : public ObjectContainer {
      public:
        /**
         * @brief Type alias for the size type of the container.
         */
        using SizeType = std::deque<void*>::size_type;

        /**
         * @brief Type alias for the iterator of the container.
         */
        using Iterator = std::deque<void*>::iterator;

        /**
         * @brief Type alias for the constant iterator of the container.
         */
        using ConstIterator = std::deque<void*>::const_iterator;

        /**
         * @brief Initializes the object container.
         */
        FORCE_STACK_ALIGN void init() override;

        /**
         * @brief Adds an object to the container.
         *
         * @param object Pointer to the object being added.
         *
         * @return Returns \c true if the object was added successfully, \c false otherwise.
         */
        FORCE_STACK_ALIGN bool add(void* object) override;

        /**
         * @brief Removes an object from the container.
         *
         * @param object Pointer to the object being removed.
         *
         * @return Returns \c true if the object was removed successfully, \c false otherwise.
         */
        FORCE_STACK_ALIGN bool remove(void* object) override;

        /**
         * @brief Clears the container.
         *
         * @param free_elements_memory Specifies whether to free the memory occupied by the objects.
         */
        FORCE_STACK_ALIGN void clear(bool free_elements_memory) override;

        /**
         * @brief Retrieves the first object in the container.
         *
         * @return Pointer to the first object in the container.
         */
        FORCE_STACK_ALIGN void* first() override;

        /**
         * @brief Retrieves the next object in the container.
         *
         * @return Pointer to the next object in the container.
         */
        FORCE_STACK_ALIGN void* next() override;

        /**
         * @brief Retrieves the size of the container.
         *
         * @return The size of the container (number of objects).
         */
        [[nodiscard]] FORCE_STACK_ALIGN SizeType size() const override;

        /**
         * @brief Checks if the container contains the specified object.
         *
         * @param object Pointer to the object being checked.
         *
         * @return Returns \c true if the object is present in the container, \c false otherwise.
         */
        FORCE_STACK_ALIGN bool contains(void* object) override;

        /**
         * @brief Checks if the container is empty.
         *
         * @return Returns \c true if the container is empty, \c false otherwise.
         */
        [[nodiscard]] FORCE_STACK_ALIGN bool empty() const override;

        /**
         * @brief Adds an object to the beginning of the container.
         *
         * @param object Pointer to the object being added.
         *
         * @return Returns \c true if the object was added successfully, \c false otherwise.
         */
        bool add_head(void* object);

        /**
         * @brief Adds an object to the end of the container.
         *
         * @param object Pointer to the object being added.
         *
         * @return Returns \c true if the object was added successfully, \c false otherwise.
         */
        bool add_tail(void* object);

        /**
         * @brief Removes the object at the beginning of the container.
         *
         * @return Pointer to the object that was removed.
         */
        void* remove_head();

        /**
         * @brief Removes the object at the end of the container.
         *
         * @return Pointer to the object that was removed.
         */
        void* remove_tail();

        /**
         * @brief Accesses an element at a specified index in the container.
         *
         * @param index Index of the element to access.
         *
         * @return Reference to the element at the specified index.
         */
        [[nodiscard]] void*& operator[](SizeType index);

        /**
         * @brief Accesses an element at a specified index in the container.
         *
         * @param index Index of the element to access.
         *
         * @return Reference to the element at the specified index.
         */
        [[nodiscard]] void*& operator[](int index);

        /**
         * @brief Accesses an element at a specified index in the container
         *
         * @param index Index of the element to access.
         *
         * @return Element at the specified index.
         */
        [[nodiscard]] void* operator[](SizeType index) const;

        /**
         * @brief Accesses an element at a specified index in the container
         *
         * @param index Index of the element to access.
         *
         * @return Element at the specified index.
         */
        [[nodiscard]] void* operator[](int index) const;

        /**
         * @brief Returns an iterator to the beginning of the container.
         *
         * @return Iterator to the beginning of the container.
         */
        [[nodiscard]] auto begin() noexcept;

        /**
         * @brief Returns an iterator to the beginning of the container.
         *
         * @return Const iterator to the beginning of the container.
         */
        [[nodiscard]] auto cbegin() const noexcept;

        /**
         * @brief Returns a reverse iterator to the beginning of the container.
         *
         * @return Reverse iterator to the beginning of the container.
         */
        [[nodiscard]] auto rbegin() noexcept;

        /**
         * @brief Returns a reverse iterator to the beginning of the container.
         *
         * @return Const reverse iterator to the beginning of the container.
         */
        [[nodiscard]] auto crbegin() const noexcept;

        /**
         * @brief Returns an iterator to the end of the container.
         *
         * @return Iterator to the end of the container.
         */
        [[nodiscard]] auto end() noexcept;

        /**
         * @brief Returns an iterator to the end of the container.
         *
         * @return Const iterator to the end of the container.
         */
        [[nodiscard]] auto cend() const noexcept;

        /**
         * @brief Returns a reverse iterator to the end of the container.
         *
         * @return Reverse iterator to the end of the container.
         */
        [[nodiscard]] auto rend() noexcept;

        /**
         * @brief Returns a reverse iterator to the end of the container.
         *
         * @return Const reverse iterator to the end of the container.
         */
        [[nodiscard]] auto crend() const noexcept;

        /**
         * @brief Erases an element from the container.
         *
         * @param pos Iterator to the element to erase.
         *
         * @return Iterator following the last removed element.
         */
        Iterator erase(const ConstIterator& pos);

        /**
         * @brief Erases a range of elements from the container.
         *
         * @param first Iterator to the first element to erase.
         * @param last Iterator to the element following the last element to erase.
         *
         * @return Iterator following the last removed element.
         */
        Iterator erase(const ConstIterator& first, const ConstIterator& last);

        /**
         * @brief Sorts the elements in the container.
         *
         * @tparam Comparator Type of the comparator function object.
         *
         * @param comp Comparator function object that defines the order of elements.
         */
        template <typename Comparator>
        void sort(Comparator&& comp);

      private:
        /// Current size of the container.
        SizeType current_{};

        /// Internal container storing the objects.
        std::deque<void*> container_{};
    };

    inline void*& ObjectList::operator[](const SizeType index)
    {
        return container_[index];
    }

    inline void*& ObjectList::operator[](const int index)
    {
        return container_[static_cast<SizeType>(index)];
    }

    inline void* ObjectList::operator[](const SizeType index) const
    {
        return container_[index];
    }

    inline void* ObjectList::operator[](const int index) const
    {
        return container_[static_cast<SizeType>(index)];
    }

    inline auto ObjectList::begin() noexcept
    {
        return container_.begin();
    }

    inline auto ObjectList::cbegin() const noexcept
    {
        return container_.cbegin();
    }

    inline auto ObjectList::rbegin() noexcept
    {
        return container_.rbegin();
    }

    inline auto ObjectList::crbegin() const noexcept
    {
        return container_.crbegin();
    }

    inline auto ObjectList::end() noexcept
    {
        return container_.end();
    }

    inline auto ObjectList::cend() const noexcept
    {
        return container_.cend();
    }

    inline auto ObjectList::rend() noexcept
    {
        return container_.rend();
    }

    inline auto ObjectList::crend() const noexcept
    {
        return container_.crend();
    }

    inline ObjectList::Iterator ObjectList::erase(const ObjectList::ConstIterator& pos)
    {
        current_ = 0;
        return container_.erase(pos);
    }

    inline ObjectList::Iterator
      ObjectList::erase(const ObjectList::ConstIterator& first, const ObjectList::ConstIterator& last)
    {
        current_ = 0;
        return container_.erase(first, last);
    }

    template <typename Comparator>
    void ObjectList::sort(Comparator&& comp)
    {
        current_ = 0;
        std::sort(container_.begin(), container_.end(), std::forward<Comparator>(comp));
    }
}
