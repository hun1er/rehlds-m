/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "common/platform.hpp"

namespace common
{
    /**
     * @brief This is the interface exported by the engine
     * to allow a dedicated server front end application to host it.
     */
    class NO_VTABLE DedicatedServerApiInterface : public CommonInterface
    {
      public:
        /**
         * @brief The string name of this interface.
         */
        static constexpr auto* INTERFACE_NAME = "VENGINE_HLDS_API_VERSION002";

        /**
         * @brief Initialize the engine with the provided parameters.
         */
        virtual bool init(const char* basedir, const char* cmdline, CreateInterfaceFunc hlds_factory,
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
