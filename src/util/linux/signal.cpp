/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/linux/signal.hpp"
#include "util/linux/system/error.hpp"
#include "util/logger.hpp"
#include <cerrno>
#include <cstring>
#include <string>

namespace
{
    util::SignalErrorCode get_error_code() noexcept
    {
        switch (errno) {
            case EFAULT: return util::SignalErrorCode::fault;
            case EINTR: return util::SignalErrorCode::interrupted;
            case EINVAL: return util::SignalErrorCode::invalid_argument;
            default: return util::SignalErrorCode::unknown;
        }
    }

    template <typename T>
    std::string get_signal_names(const T& signals)
    {
        std::string signal_names{};

        for (const auto signal : signals) {
            const auto* const name = ::strsignal(signal);

            if ((nullptr == name) || ('\0' == *name)) {
                continue;
            }

            if (!signal_names.empty()) {
                signal_names.append(", ");
            }

            signal_names.append(name);
        }

        return signal_names;
    }

    std::string get_signal_names(const sigset_t& set)
    {
        std::string signal_names{};

        for (int signal = 1; signal < NSIG; ++signal) {
            if (::sigismember(&set, signal) != 1) {
                continue;
            }

            if (const auto* const name = ::strsignal(signal); (name != nullptr) && (*name != '\0')) {
                if (!signal_names.empty()) {
                    signal_names.append(", ");
                }

                signal_names.append(name);
            }
        }

        return signal_names;
    }
}

namespace util
{
    SignalErrorCode create_signal_set(const std::initializer_list<int>& signals, ::sigset_t& set) noexcept
    {
        if (::sigemptyset(&set) != 0) {
            return get_error_code();
        }

        for (const auto signal : signals) {
            // cppcheck-suppress useStlAlgorithm
            if (::sigaddset(&set, signal) != 0) {
                return get_error_code();
            }
        }

        return SignalErrorCode::success;
    }

    SignalErrorCode change_signal_mask(const int how, const sigset_t& set, sigset_t& old_set) noexcept
    {
        return 0 == ::sigprocmask(how, &set, &old_set) ? util::SignalErrorCode::success : get_error_code();
    }

    SignalErrorCode change_signal_mask(const int how, const sigset_t& set) noexcept
    {
        return 0 == ::sigprocmask(how, &set, nullptr) ? util::SignalErrorCode::success : get_error_code();
    }

    bool try_create_signal_set(const std::initializer_list<int>& signals, ::sigset_t& set) noexcept
    {
        if (SignalErrorCode::success == create_signal_set(signals, set)) {
            return true;
        }

        const auto signal_names = get_signal_names(signals);
        const auto errno_string = util::get_last_error_string();
        log_error("Failed to create signal set for signals [{}]: {}", signal_names, errno_string);

        return false;
    }

    bool try_change_signal_mask(const int how, const ::sigset_t& set, ::sigset_t& old_set) noexcept
    {
        if (util::SignalErrorCode::success == change_signal_mask(how, set, old_set)) {
            return true;
        }

        const auto signal_names = get_signal_names(set);
        const auto errno_string = util::get_last_error_string();
        log_error("Failed to change signal mask for signals [{}]: {}", signal_names, errno_string);

        return false;
    }

    bool try_change_signal_mask(const int how, const ::sigset_t& set) noexcept
    {
        ::sigset_t old_set{};
        return try_change_signal_mask(how, set, old_set);
    }
}
