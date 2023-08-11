/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/userinput_history.hpp"
#include "util/file.hpp"
#include "util/string.hpp"
#include <string_view>
#include <vector>

namespace
{
    /// A vector to store history lines when saving to file.
    std::vector<std::string> history_lines{};

    /// Check if the input is valid.
    bool is_valid_input(const std::string_view input) noexcept
    {
        return (!util::str::is_empty_or_space(input)) && util::str::is_printable(input);
    }
}

namespace model
{
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

        if (util::file_exists(filename_)) {
            util::try_file_remove(filename_);
        }
    }

    void UserInputHistory::load()
    {
        if (!util::file_exists(filename_)) {
            return;
        }

        if (std::string content{}; util::try_file_read(filename_, content)) {
            history_.clear();

            for (const auto& input : util::str::split_lines(content)) {
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

        util::try_file_write_lines(filename_, history_lines, false);
    }
}
