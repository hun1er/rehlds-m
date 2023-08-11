/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/cmdline_args.hpp"
#include "util/file.hpp"
#include "util/string.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string_view>

namespace
{
    [[nodiscard]] constexpr auto is_special_char(const std::string::value_type ch) noexcept
    {
        return ('-' == ch) || ('+' == ch) || ('@' == ch);
    }

    bool is_valid_param(const std::string_view param) noexcept
    {
        return (param.length() > 1) && (is_special_char(param.front()));
    }

    std::string_view normalize_param_name(std::string_view param)
    {
        if (const auto first_char = param.front(); ('+' == first_char) || ('-' == first_char)) {
            param.remove_prefix(1);
        }

        return param;
    }

    std::string::size_type find_param_pos(const std::string_view params, const std::string_view param) noexcept
    {
        const auto norm_param = normalize_param_name(param);
        const auto pos1 = params.rfind("+" + std::string{norm_param});
        const auto pos2 = params.rfind("-" + std::string{norm_param});

        if (std::string::npos == pos1) {
            return pos2;
        }

        if (std::string::npos == pos2) {
            return pos1;
        }

        return std::max(pos1, pos2);
    }

    std::optional<std::string> extract_param_option(const std::string_view params, const std::string_view param_name,
                                                    const std::size_t pos) noexcept
    {
        if (pos != std::string::npos) {
            auto new_pos = pos + param_name.size() + 1;

            while ((new_pos < params.size()) && (' ' == params[new_pos])) {
                ++new_pos;
            }

            if ((new_pos < params.size()) && (params[new_pos] != '+') && (params[new_pos] != '-')) {
                auto end_pos = new_pos;

                while ((end_pos < params.size()) && (params[end_pos] != ' ')) {
                    ++end_pos;
                }

                return std::string{params.substr(new_pos, end_pos - new_pos)};
            }
        }

        return std::nullopt;
    }
}

namespace model
{
    CmdLineArgs::CmdLineArgs(const int argc, const char* const* const argv)
    {
        assert(argc >= 0);

        std::vector<std::string> args{};
        args.reserve(static_cast<std::size_t>(argc));

        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        parse(args);
    }

    void CmdLineArgs::push_front(std::string param, std::string option)
    {
        if (param = util::str::trim(param); !is_valid_param(param)) {
            assert(false);
            return;
        }

        remove(param);

        if (option = util::str::trim(option); option.empty()) {
            params_.insert(0, param + ' ');
        }
        else {
            params_.insert(0, param + ' ' + option + ' ');
        }

        params_ = util::str::trim(params_);
    }

    void CmdLineArgs::push_back(std::string param, std::string option)
    {
        if (param = util::str::trim(param); !is_valid_param(param)) {
            assert(false);
            return;
        }

        remove(param);

        if (option = util::str::trim(option); option.empty()) {
            params_.append(' ' + param);
        }
        else {
            params_.append(' ' + param + ' ' + option);
        }

        params_ = util::str::trim(params_);
    }

    void CmdLineArgs::remove(const std::string_view param)
    {
        const auto norm_param = normalize_param_name(param);
        auto pos = std::string::npos;

        // cppcheck-suppress redundantInitialization
        while ((pos = find_param_pos(params_, norm_param)) != std::string::npos) {
            auto num_chars_to_remove = 1 + norm_param.length() + 1;

            if (const auto option = extract_param_option(params_, param, pos); option) {
                num_chars_to_remove += option->length() + 1;
            }

            if (pos > 0) {
                --pos;
            }

            params_.erase(pos, num_chars_to_remove);
        }
    }

    bool CmdLineArgs::contains(const std::string_view param) const noexcept
    {
        return find_param_pos(params_, param) != std::string::npos;
    }

    int CmdLineArgs::find_param_index(const int argc, const char* const* const argv,
                                      const std::string_view param) noexcept
    {
        if (argc > 1) {
            const auto norm_param = normalize_param_name(param);
            const auto plus_param = "+" + std::string{norm_param};
            const auto minus_param = "-" + std::string{norm_param};

            for (auto i = argc - 1; i >= 1; --i) {
                if (util::str::equal(argv[i], minus_param) || util::str::equal(argv[i], plus_param)) {
                    return i;
                }
            }
        }

        return -1;
    }

    std::optional<std::string> CmdLineArgs::get_param_option(const int argc, const char* const* const argv,
                                                             const std::string_view param) noexcept
    {
        int index = find_param_index(argc, argv, param);

        if (index == -1) {
            return std::nullopt;
        }

        if (index += 1; (index >= argc) || is_special_char(*argv[index])) {
            return std::nullopt;
        }

        std::string option{argv[index]};
        index += 1;

        while ((index < argc) && (!is_special_char(*argv[index]))) {
            option.push_back(' ');
            option.append(argv[index]);
            index += 1;
        }

        return option;
    }

    std::optional<std::string> CmdLineArgs::get_param_option(std::string param) const
    {
        param = util::str::trim(param);

        if (param.empty()) {
            assert(false);
            return std::nullopt;
        }

        return extract_param_option(params_, param, find_param_pos(params_, param));
    }

    void CmdLineArgs::parse(const std::vector<std::string>& args) noexcept
    {
        for (const auto& arg : args) {
            if ((arg.empty()) || ((1U == arg.length()) && (is_special_char(arg.front())))) {
                continue;
            }

            if ('@' == arg.front()) {
                if (std::string file_content{}; util::try_file_read(arg.substr(1), file_content)) {
                    parse(util::str::split(file_content));
                }
                else {
                    continue;
                }
            }
            else {
                params_.append(arg);
            }

            params_.push_back(' ');
        }

        params_ = util::str::trim(params_);
    }
}
