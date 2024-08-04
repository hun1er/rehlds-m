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

#include <string>
#include <string_view>

namespace View {
    /**
     * @brief An abstract interface for displaying information.
     */
    class BaseView {
      public:
        /**
         * @brief Default constructor.
         */
        BaseView() = default;

        /**
         * @brief Move constructor.
         */
        BaseView(BaseView&&) = default;

        /**
         * @brief Copy constructor.
         */
        BaseView(const BaseView&) = default;

        /**
         * @brief Move assignment operator.
         */
        BaseView& operator=(BaseView&&) = default;

        /**
         * @brief Copy assignment operator.
         */
        BaseView& operator=(const BaseView&) = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~BaseView() = default;

        /**
         * @brief Prints the given text.
         *
         * @param text The text to be printed.
         *
         * @return 0 if the operation is successful.
         */
        virtual int print(std::string_view text) = 0;

        /**
         * @brief Displays the server status information.
         *
         * @param fps The current FPS of the server.
         * @param num_players The number of active players on the server.
         * @param max_players The maximum number of players allowed on the server.
         * @param map The name of the current map on the server.
         */
        virtual void display_server_status(float fps, int num_players, int max_players, const std::string& map) = 0;
    };
}
