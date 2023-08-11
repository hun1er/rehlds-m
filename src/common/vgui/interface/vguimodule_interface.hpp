/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "common/platform.hpp"
#include <cstdint>

namespace common::vgui
{
    /**
     * @brief Handle to an internal VGui panel.
     * This is the only handle to a panel that is valid across dll boundaries.
     */
    using VPanel = std::uint32_t;

    //
    // Handles to vgui objects.
    // NULL values signify an invalid value.
    //

    using HScheme = unsigned long;  // NOLINT(*-runtime-int)
    using HTexture = unsigned long; // NOLINT(*-runtime-int)
    using HCursor = unsigned long;  // NOLINT(*-runtime-int)
    using HPanel = unsigned long;   // NOLINT(*-runtime-int)
    using HFont = unsigned long;    // NOLINT(*-runtime-int)

    constexpr HPanel INVALID_PANEL = 0xFFFF'FFFFU;
    constexpr HFont INVALID_FONT = 0;
    constexpr auto STEREO_NOOP = 1.F;
    constexpr auto STEREO_INVALID = 0.F;
}

namespace common
{
    /**
     * @brief Standard interface to loading VGui modules.
     */
    class NO_VTABLE VGuiModuleInterface : public CommonInterface
    {
      public:
        /**
         * @brief The string name of this interface.
         */
        static constexpr auto* INTERFACE_NAME = "VGuiModule002";

        /**
         * @brief Called first to setup the module with the VGui.
         *
         * @return \c true on success, \c false on failure.
         */
        virtual bool initialize(CreateInterfaceFunc* vgui_factories, int factory_count) = 0;

        /**
         * @brief Called after all the modules have been initialized.
         *
         * @note Modules should use this time to link to all the other module interfaces.
         */
        virtual bool post_initialize(CreateInterfaceFunc* modules, int factory_count) = 0;

        /**
         * @brief Called when the module is selected from the menu or otherwise activated.
         */
        virtual bool activate() = 0;

        /**
         * @brief Returns \c true if the module is successfully initialized and available.
         */
        virtual bool is_valid() = 0;

        /**
         * @brief Requests that the UI is temporarily disabled and all data files saved.
         */
        virtual void deactivate() = 0;

        /**
         * @brief Restart from a \c deactivate()
         */
        virtual void reactivate() = 0;

        /**
         * @brief Called when the module is about to be shutdown.
         */
        virtual void shutdown() = 0;

        /**
         * @brief Returns a handle to the main module panel.
         */
        virtual vgui::VPanel get_panel() = 0;

        /**
         * @brief Sets the parent of the main module panel.
         */
        virtual void set_parent(vgui::VPanel parent) = 0;

        // Messages sent through the panel returned by GetPanel():
        //
        // "ConnectedToGame"        "ip"    "port"  "gamedir"
        // "DisconnectedFromGame"
        // "ActiveGameName"         "name"
        // "LoadingStarted"         "type"  "name"
        // "LoadingFinished"        "type"  "name"
    };
}
