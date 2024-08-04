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

#include "model/userinput_history.hpp"
#include "util/file.hpp"
#include "util/string.hpp"
#include <string_view>
#include <vector>

namespace {
    /// A vector to store history lines when saving to file.
    std::vector<std::string> history_lines{};

    /// Check if the input is valid.
    bool is_valid_input(const std::string_view input) noexcept
    {
        return (!Util::str::is_empty_or_space(input)) && Util::str::is_printable(input);
    }
}

namespace Model {
    UserInputHistory::UserInputHistory(const std::string_view filename) : filename_(filename)
    {
        history_lines.reserve(HISTORY_CAPACITY);
    }

    void UserInputHistory::append(const std::string_view input)
    {
        if (is_valid_input(input) && (input != history_.back())) {
            history_.emplace_back(input);
        }

        position_ = history_.size();
    }

    std::optional<std::string> UserInputHistory::next() noexcept
    {
        if (position_ < history_.size()) {
            ++position_;
        }

        if (position_ < history_.size()) {
            return history_[position_];
        }

        return std::nullopt;
    }

    std::optional<std::string> UserInputHistory::previous() noexcept
    {
        if (position_ > 0) {
            --position_;
            return history_[position_];
        }

        return std::nullopt;
    }

    void UserInputHistory::clear()
    {
        history_.clear();
        position_ = history_.size();

        if (Util::file_exists(filename_)) {
            Util::try_file_remove(filename_);
        }
    }

    void UserInputHistory::load()
    {
        if (!Util::file_exists(filename_)) {
            return;
        }

        if (std::string content{}; Util::try_file_read(filename_, content)) {
            history_.clear();

            for (const auto& input : Util::str::split_lines(content)) {
                if (is_valid_input(input) && (input != history_.back())) {
                    history_.emplace_back(input); // cppcheck-suppress useStlAlgorithm
                }
            }

            position_ = history_.size();
        }
    }

    void UserInputHistory::save()
    {
        if (filename_.empty() || history_.empty()) {
            return;
        }

        history_lines.clear();
        const auto history_size = history_.size();

        for (SizeType i = 0; i < history_size; ++i) {
            history_lines.emplace_back(history_[i]);
        }

        Util::try_file_write_lines(filename_, history_lines, false);
    }
}
