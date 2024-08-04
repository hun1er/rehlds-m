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

#include "common/interface.hpp"
#include "common/platform.hpp"

namespace Common {
    /**
     * @brief This is the interface exported by the engine
     * to allow a dedicated server front end application to host it.
     */
    class NO_VTABLE DedicatedServerApiInterface : public CommonInterface {
      public:
        /**
         * @brief The string name of this interface.
         */
        static constexpr auto* INTERFACE_NAME = "VENGINE_HLDS_API_VERSION002";

        /**
         * @brief Initialize the engine with the provided parameters.
         */
        virtual bool init(
          const char* basedir, const char* cmdline, CreateInterfaceFunc hlds_factory,
          CreateInterfaceFunc filesystem_factory) = 0;

        /**
         * @brief Shuts down the dedicated server.
         */
        virtual int shutdown() = 0;

        /**
         * @brief Run a frame.
         */
        virtual bool run_frame() = 0;

        /**
         * @brief Insert text into console.
         */
        virtual void add_console_text(const char* text) = 0;

        /**
         * @brief Get current status to display in the HLDS UI (console window title bar, e.g.).
         */
        virtual void update_status(float* fps, int* active_players, int* max_players, char* current_map) = 0;
    };
}
