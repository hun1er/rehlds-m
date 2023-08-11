/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <csignal>
#include <initializer_list>

namespace util
{
    /**
     * @brief Possible error codes for signal-related functions.
     */
    enum class SignalErrorCode
    {
        success,          ///< No error occurred
        fault,            ///< An invalid memory address was specified
        interrupted,      ///< The function was interrupted by a signal
        invalid_argument, ///< An invalid argument was specified
        unknown           ///< An unknown error occurred
    };

    /**
     * @brief Creates a signal set with the specified signals.
     *
     * @param signals An initializer list of signals to add to the set.
     * @param set The signal set to initialize.
     *
     * @return An \c util::signal::SignalErrorCode value indicating the result of the operation.
     */
    SignalErrorCode create_signal_set(const std::initializer_list<int>& signals, ::sigset_t& set) noexcept;

    /**
     * @brief Changes the signal mask of the calling thread.
     *
     * @param how Specifies how the signal mask should be changed.
     * @param set The set of signals to change.
     * @param old_set The previous value of the signal mask is stored here.
     *
     * @return An \c util::signal::SignalErrorCode value indicating the result of the operation.
     */
    SignalErrorCode change_signal_mask(int how, const ::sigset_t& set, ::sigset_t& old_set) noexcept;

    /**
     * @brief Changes the signal mask of the calling thread without storing the previous value.
     *
     * @param how Specifies how the signal mask should be changed.
     * @param set The set of signals to change.
     *
     * @return An \c util::signal::SignalErrorCode value indicating the result of the operation.
     */
    SignalErrorCode change_signal_mask(int how, const ::sigset_t& set) noexcept;

    /**
     * @brief Tries to create a signal set with the specified signals and logs an error message on failure.
     *
     * @param signals An initializer list of signals to add to the set.
     * @param set The signal set to initialize.
     *
     * @return \c true on success, \c false on failure.
     */
    bool try_create_signal_set(const std::initializer_list<int>& signals, ::sigset_t& set) noexcept;

    /**
     * @brief Tries to change the signal mask of the calling thread and logs an error message on failure.
     *
     * @param how Specifies how the signal mask should be changed.
     * @param set The set of signals to change.
     * @param old_set The previous value of the signal mask is stored here.
     *
     * @return \c true on success, \c false on failure.
     */
    bool try_change_signal_mask(int how, const ::sigset_t& set, ::sigset_t& old_set) noexcept;

    /**
     * @brief Tries to change the signal mask of the calling thread
     * without storing the previous value and logs an error message on failure.
     *
     * @param how Specifies how the signal mask should be changed.
     * @param set The set of signals to change.
     *
     * @return \c true on success, \c false on failure.
     */
    bool try_change_signal_mask(int how, const ::sigset_t& set) noexcept;

    /**
     * @brief A RAII class for blocking signals within a scope.
     *
     * @tparam LogErrors Indicates whether to log errors when blocking or unblocking signals.
     */
    template <bool LogErrors = true>
    class ScopedSignalBlocker final
    {
      public:
        /**
         * @brief Construct a new ScopedSignalBlocker object and block the specified signals.
         *
         * @param signals An initializer list of signals to block.
         */
        ScopedSignalBlocker(const std::initializer_list<int>& signals)
        {
            if constexpr (LogErrors) {
                if (try_create_signal_set(signals, set_)) {
                    blocked_ = try_change_signal_mask(SIG_BLOCK, set_);
                }
            }
            else {
                if (SignalErrorCode::success == create_signal_set(signals, set_)) {
                    blocked_ = SignalErrorCode::success == change_signal_mask(SIG_BLOCK, set_);
                }
            }
        }

        /// Move constructor.
        ScopedSignalBlocker(ScopedSignalBlocker&&) = delete;

        /// Copy constructor.
        ScopedSignalBlocker(const ScopedSignalBlocker&) = delete;

        /// Move assignment operator.
        ScopedSignalBlocker& operator=(ScopedSignalBlocker&&) = delete;

        /// Copy assignment operator.
        ScopedSignalBlocker& operator=(const ScopedSignalBlocker&) = delete;

        /**
         * @brief Destroy the ScopedSignalBlocker object and unblock the previously blocked signals.
         */
        ~ScopedSignalBlocker()
        {
            if constexpr (LogErrors) {
                if (blocked_) {
                    try_change_signal_mask(SIG_UNBLOCK, set_);
                }
            }
            else {
                if (blocked_) {
                    change_signal_mask(SIG_UNBLOCK, set_);
                }
            }
        }

      private:
        /// Indicates whether the signals were successfully blocked.
        bool blocked_{};

        /// The set of signals that were blocked.
        ::sigset_t set_{};
    };
}
