/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "util/singleton.hpp"

namespace util
{
    /**
     * @brief A class for creating crash dump files.
     */
    class CrashDumper final
    {
        friend class policies::CreateUnique<CrashDumper>;

      public:
        /**
         * @brief Destructor.
         */
        ~CrashDumper();

        /// Move constructor.
        CrashDumper(CrashDumper&&) = delete;

        /// Copy constructor.
        CrashDumper(const CrashDumper&) = delete;

        /// Move assignment operator.
        CrashDumper& operator=(CrashDumper&&) = delete;

        /// Copy assignment operator.
        CrashDumper& operator=(const CrashDumper&) = delete;

        /**
         * @brief Initializes the CrashDumper.
         */
        static void init();

      private:
        /**
         * @brief Constructor.
         */
        CrashDumper();
    };
}
