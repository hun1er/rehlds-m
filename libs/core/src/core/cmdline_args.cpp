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

#include "core/cmdline_args.hpp"
#include "util/file.hpp"
#include "util/string.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace {
    constexpr auto FIRST_ARG_INDEX = 1; // Index of the first argument, skipping the executable name

    [[nodiscard]] constexpr auto is_prefix_char(const std::string::value_type ch) noexcept -> bool
    {
        return ('-' == ch) || ('+' == ch) || ('@' == ch);
    }

    [[nodiscard]] constexpr auto is_valid_argument(const std::string_view arg) noexcept -> bool
    {
        return (arg.length() > 1) && (is_prefix_char(arg.front()));
    }

    std::string remove_argument_prefix(std::string_view arg)
    {
        if (const auto first_char = arg.front(); is_prefix_char(first_char)) {
            arg.remove_prefix(1);
        }

        return std::string{arg};
    }

    [[nodiscard]] std::vector<std::string> extract_cmdline_args(const int argc, const char* const* const argv)
    {
        if (argc <= FIRST_ARG_INDEX) {
            return {};
        }

        std::vector<std::string> args{};
        args.reserve(static_cast<std::size_t>(argc - FIRST_ARG_INDEX));

        for (int i = FIRST_ARG_INDEX; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        return args;
    }

    std::string build_arguments_string(const std::vector<std::string>& args)
    {
        std::string string{};

        for (const auto& arg : args) {
            if ((arg.empty()) || ((1U == arg.length()) && (is_prefix_char(arg.front())))) {
                continue;
            }

            if ('@' == arg.front()) {
                std::string file_content{};

                if (Util::FileReadErrorCode::success == Util::file_read(arg.substr(1), file_content)) {
                    string += build_arguments_string(Util::str::split(file_content));
                }
                else {
                    continue;
                }
            }
            else {
                string.append(arg);
            }

            string.push_back(' ');
        }

        return Util::str::trim(string);
    }
}

namespace Core {
    CmdLineArgs CmdLineArgs::from_command_line(const int argc, const char* const* const argv)
    {
        const auto& args = extract_cmdline_args(argc, argv);
        auto args_string = build_arguments_string(args);

        return CmdLineArgs(std::move(args_string));
    }

    auto CmdLineArgs::find_argument_index(const int argc, const char* const* const argv, std::string arg) -> int
    {
        arg = remove_argument_prefix(arg);

        const auto plus_arg = "+" + arg;
        const auto minus_arg = "-" + arg;

        for (auto i = FIRST_ARG_INDEX; i < argc; ++i) {
            if (Util::str::equal(argv[i], minus_arg) || Util::str::equal(argv[i], plus_arg)) {
                return i;
            }
        }

        return -1;
    }

    auto CmdLineArgs::contains(const int argc, const char* const* const argv, std::string arg) -> bool
    {
        return find_argument_index(argc, argv, std::move(arg)) != -1;
    }

    void CmdLineArgs::push_front(std::string arg, std::string option)
    {
        if (arg = Util::str::trim(arg); !is_valid_argument(arg)) {
            return;
        }

        remove(arg);

        if (option = Util::str::trim(option); option.empty()) {
            arguments_.insert(0, arg + ' ');
        }
        else {
            arguments_.insert(0, arg + ' ' + option + ' ');
        }

        arguments_ = Util::str::trim(arguments_);
    }

    void CmdLineArgs::push_back(std::string arg, std::string option)
    {
        if (arg = Util::str::trim(arg); !is_valid_argument(arg)) {
            return;
        }

        remove(arg);

        if (option = Util::str::trim(option); option.empty()) {
            arguments_.append(' ' + arg);
        }
        else {
            arguments_.append(' ' + arg + ' ' + option);
        }

        arguments_ = Util::str::trim(arguments_);
    }

    void CmdLineArgs::remove(std::string arg)
    {
        arg = remove_argument_prefix(arg);
        auto pos = std::string::npos;

        while ((pos = find_argument_pos(arg)) != std::string::npos) {
            auto num_chars_to_remove = 1 + arg.length() + 1;

            if (const auto option = get_argument_option(arg, pos); option) {
                num_chars_to_remove += option->length() + 1;
            }

            if (pos > 0) {
                --pos;
            }

            arguments_.erase(pos, num_chars_to_remove);
        }
    }

    auto CmdLineArgs::find_argument_pos(std::string arg) const -> std::size_t
    {
        arg = remove_argument_prefix(arg);

        const auto pos1 = arguments_.rfind("+" + arg);
        const auto pos2 = arguments_.rfind("-" + arg);

        if (std::string::npos == pos1) {
            return pos2;
        }

        if (std::string::npos == pos2) {
            return pos1;
        }

        return std::max(pos1, pos2);
    }

    auto CmdLineArgs::get_argument_option(std::string arg) const -> std::optional<std::string>
    {
        const auto pos = find_argument_pos(arg);
        return get_argument_option(arg, pos);
    }

    auto CmdLineArgs::get_argument_option(std::string arg, const std::size_t pos) const -> std::optional<std::string>
    {
        if (pos != std::string::npos) {
            auto new_pos = pos + arg.size() + 1;

            while ((new_pos < arguments_.size()) && (' ' == arguments_[new_pos])) {
                ++new_pos;
            }

            if ((new_pos < arguments_.size()) && (arguments_[new_pos] != '+') && (arguments_[new_pos] != '-')) {
                auto end_pos = new_pos;

                while ((end_pos < arguments_.size()) && (arguments_[end_pos] != ' ')) {
                    ++end_pos;
                }

                return std::string{arguments_.substr(new_pos, end_pos - new_pos)};
            }
        }

        return std::nullopt;
    }
}
