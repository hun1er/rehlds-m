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

#include <string_view>

namespace Util {
    /**
     * @brief Interface for log output destinations.
     *
     * Defines the contract for classes that handle the output
     * of log messages in various formats or to different destinations.
     */
    class LogOutput {
      public:
        /**
         * @brief Default constructor.
         *
         * Initializes a new instance of the class.
         */
        LogOutput() = default;

        /**
         * @brief Move constructor.
         *
         * Transfers ownership from another instance.
         *
         * @param other The instance to move from.
         */
        LogOutput(LogOutput&& other) = default;

        /**
         * @brief Copy constructor.
         *
         * Creates a new instance as a copy of another instance.
         *
         * @param other The instance to copy from.
         */
        LogOutput(const LogOutput& other) = default;

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         *
         * Ensures that when a derived class object is deleted through a pointer to LogOutput,
         * the destructor of the derived class is invoked, allowing for proper resource cleanup.
         */
        virtual ~LogOutput() = default;

        /**
         * @brief Move assignment operator.
         *
         * Transfers ownership from another instance through assignment.
         *
         * @param other The instance to move from.
         *
         * @return A reference to the assigned instance.
         */
        LogOutput& operator=(LogOutput&& other) = default;

        /**
         * @brief Copy assignment operator.
         *
         * Copies the state of another instance through assignment.
         *
         * @param other The instance to copy from.
         *
         * @return A reference to the assigned instance.
         */
        LogOutput& operator=(const LogOutput& other) = default;

        /**
         * @brief Write a log message to the output destination.
         *
         * @param message The formatted log message to be written.
         */
        virtual void write_log(std::string_view message) = 0;
    };
}
