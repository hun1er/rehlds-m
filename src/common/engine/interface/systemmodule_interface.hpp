/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "common/platform.hpp"

namespace common
{
    class NO_VTABLE SystemModuleInterface : public CommonInterface
    {
      public:
        /**
         * @brief The string name of this interface.
         */
        static constexpr auto* INTERFACE_NAME = "basesystem002";
        virtual bool init(class SystemInterface* system, int serial, const char* name) = 0;
        virtual void run_frame(double time) = 0;
        virtual void receive_signal(SystemModuleInterface* module, unsigned int signal, void* data) = 0;
        virtual void execute_command(int command_id, const char* command_line) = 0;
        virtual void register_listener(SystemModuleInterface* module) = 0;
        virtual void remove_listener(SystemModuleInterface* module) = 0;
        virtual SystemInterface* get_system() = 0;
        virtual int get_serial() = 0;
        virtual char* get_status_line() = 0;
        virtual char* get_type() = 0;
        virtual char* get_name() = 0;
        virtual int get_state() = 0;
        virtual int get_version() = 0;
        virtual void shutdown() = 0;
    };
}
