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
    class NO_VTABLE SystemModuleInterface : public CommonInterface {
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
