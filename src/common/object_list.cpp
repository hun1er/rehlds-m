/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "common/object_list.hpp"
#include <algorithm>
#include <cstdlib>

namespace common
{
    void ObjectList::init()
    {
        current_ = 0;
        clear(false);
    }

    bool ObjectList::add(void* const object)
    {
        return add_tail(object);
    }

    bool ObjectList::remove(void* const object)
    {
        const auto& it = std::find(container_.begin(), container_.end(), object);

        if (it == container_.end()) {
            return false;
        }

        if ((current_ > 0) && (static_cast<SizeType>(std::distance(container_.begin(), it)) < current_)) {
            --current_;
        }

        container_.erase(it);

        return true;
    }

    void ObjectList::clear(const bool free_elements_memory)
    {
        if (free_elements_memory) {
            for (auto* const object : container_) {
                std::free(object); // NOLINT
            }
        }

        current_ = 0;
        container_.clear();
    }

    void* ObjectList::first()
    {
        void* object = nullptr;

        if (container_.empty()) {
            current_ = 0;
        }
        else {
            current_ = 1;
            object = container_.front();
        }

        return object;
    }

    void* ObjectList::next()
    {
        if (current_ >= container_.size()) {
            return nullptr;
        }

        auto* const object = container_[current_];
        ++current_;

        return object;
    }

    ObjectList::SizeType ObjectList::size() const
    {
        return container_.size();
    }

    bool ObjectList::contains(void* const object)
    {
        if (const auto& it = std::find(container_.cbegin(), container_.cend(), object); it != container_.cend()) {
            current_ = static_cast<SizeType>(std::distance(container_.cbegin(), it));
            return true;
        }

        return false;
    }

    bool ObjectList::empty() const
    {
        return container_.empty();
    }

    bool ObjectList::add_head(void* const object)
    {
        container_.push_front(object);

        if (current_ != 0) {
            ++current_;
        }

        return true;
    }

    bool ObjectList::add_tail(void* const object)
    {
        container_.push_back(object);
        return true;
    }

    void* ObjectList::remove_head()
    {
        if (container_.empty()) {
            return nullptr;
        }

        if (current_ > 0) {
            --current_;
        }

        auto* const object = container_.front();
        container_.pop_front();

        return object;
    }

    void* ObjectList::remove_tail()
    {
        if (container_.empty()) {
            return nullptr;
        }

        if ((current_ > 0) && ((container_.size() - 1) == current_)) {
            --current_;
        }

        auto* const object = container_.back();
        container_.pop_back();

        return object;
    }
}
