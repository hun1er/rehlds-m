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

#include "common/object_list.hpp"
#include <algorithm>
#include <cstdlib>

namespace Common {
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
