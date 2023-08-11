/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/engine/interface/systemmodule_interface.hpp"
#include "common/filesystem/interface/filesystem_interface.hpp"
#include "common/object_container.hpp"
#include "common/platform.hpp"

namespace common
{
    class NO_VTABLE SystemInterface : public virtual SystemModuleInterface
    {
      public:
        virtual double get_time() = 0;
        virtual unsigned int get_tick() = 0;
        virtual void set_fps(float fps) = 0;
        virtual void print(const char* format, ...) = 0;
        virtual void print_debug(const char* format, ...) = 0;
        virtual void redirect_output(char* buffer, int max_size) = 0;
        virtual FileSystemInterface* get_file_system() = 0;
        virtual void* load_file(const char* name, int* length) = 0;
        virtual void free_file(void* handle) = 0;
        virtual void set_title(const char* title) = 0;
        virtual void set_status(const char* status) = 0;
        virtual void show_console(bool visible) = 0;
        virtual void log_console(const char* filename) = 0;
        virtual bool init_vgui(class VGuiModuleInterface* module) = 0;
#ifdef _WIN32
        virtual class Panel* get_panel() = 0;
#endif
        virtual bool register_command(const char* name, SystemModuleInterface* module, int command_id) = 0;
        virtual void get_command_matches(const char* string, ObjectContainer* match_list) = 0;
        virtual void execute_string(const char* commands) = 0;
        virtual void execute_file(const char* filename) = 0;
        virtual void print_error(const char* format, ...) = 0;
        virtual const char* check_param(const char* param) = 0;
        virtual bool add_module(SystemModuleInterface* module, const char* name) = 0;
        virtual SystemModuleInterface* get_module(const char* interface, const char* library, const char* instance) = 0;
        virtual bool remove_module(SystemModuleInterface* module) = 0;
        virtual void stop() = 0;
        virtual const char* get_base_dir() = 0;
    };
}
