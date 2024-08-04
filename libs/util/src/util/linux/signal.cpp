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

#include "util/linux/signal.hpp"
#include "util/linux/system/error.hpp"
#include "util/logger.hpp"
#include <cerrno>
#include <cstring>
#include <string>

namespace {
    Util::SignalErrorCode get_error_code() noexcept
    {
        switch (errno) {
            case EFAULT: return Util::SignalErrorCode::fault;
            case EINTR: return Util::SignalErrorCode::interrupted;
            case EINVAL: return Util::SignalErrorCode::invalid_argument;
            default: return Util::SignalErrorCode::unknown;
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

namespace Util {
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
        return 0 == ::sigprocmask(how, &set, &old_set) ? Util::SignalErrorCode::success : get_error_code();
    }

    SignalErrorCode change_signal_mask(const int how, const sigset_t& set) noexcept
    {
        return 0 == ::sigprocmask(how, &set, nullptr) ? Util::SignalErrorCode::success : get_error_code();
    }

    bool try_create_signal_set(const std::initializer_list<int>& signals, ::sigset_t& set) noexcept
    {
        if (SignalErrorCode::success == create_signal_set(signals, set)) {
            return true;
        }

        const auto signal_names = get_signal_names(signals);
        const auto errno_string = Util::get_last_error_string();
        log_error("Failed to create signal set for signals [{}]: {}", signal_names, errno_string);

        return false;
    }

    bool try_change_signal_mask(const int how, const ::sigset_t& set, ::sigset_t& old_set) noexcept
    {
        if (Util::SignalErrorCode::success == change_signal_mask(how, set, old_set)) {
            return true;
        }

        const auto signal_names = get_signal_names(set);
        const auto errno_string = Util::get_last_error_string();
        log_error("Failed to change signal mask for signals [{}]: {}", signal_names, errno_string);

        return false;
    }

    bool try_change_signal_mask(const int how, const ::sigset_t& set) noexcept
    {
        ::sigset_t old_set{};
        return try_change_signal_mask(how, set, old_set);
    }
}
