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

#pragma once

#include "util/singleton.hpp"

namespace Util {
    /**
     * @brief A class for creating crash dump files.
     */
    class CrashDumper final {
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
