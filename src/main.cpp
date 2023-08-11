/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "abstract/view.hpp"
#include "common/engine/engine_wrapper.hpp"
#include "common/filesystem/filesystem_wrapper.hpp"
#include "common/interface.hpp"
#include "console/console_input.hpp"
#include "console/console_view.hpp"
#include "model/cmdline_args.hpp"
#include "model/console_commands.hpp"
#include "model/server_loop.hpp"
#include "model/userinput_history.hpp"
#include "presenter/input_presenter.hpp"
#include "presenter/output_presenter.hpp"
#include "util/file.hpp"
#include "util/lifecycle.hpp"
#include "util/logger.hpp"
#include "util/signal.hpp"
#include "util/system.hpp"
#include <clocale>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN // NOLINT(clang-diagnostic-unused-macros)
  #include <Windows.h>
  #include <WinSock2.h>
#endif

namespace
{
    void process_cmdline_args(model::CmdLineArgs& args)
    {
        if (const auto& logfile = util::log_settings().logfile; (!logfile.empty()) && args.contains("conclearlog")) {
            util::try_file_write(logfile, std::string{}, false);
        }

        if (const auto pidfile = args.get_param_option("pidfile"); pidfile && (!pidfile->empty())) {
            const auto process_id = util::get_process_id();
            util::try_file_write(*pidfile, std::to_string(process_id) + "\n");
        }

        if (args.contains("ignoresigint")) {
            util::try_set_signal_handler(SIGINT, SIG_IGN);
        }

        if (args.contains("targetfps")) {
            args.remove("pingboost");
            args.push_back("+sys_ticrate", "0");
        }

        args.remove("condebug");
        args.push_front("-steam");

#ifdef _WIN32
        args.push_front("-console");

        if (!args.contains("nominidumps")) {
            util::CrashDumper::init();
        }
#endif
    }

    void init_locale()
    {
#ifdef _WIN32
        if (constexpr auto* locale = ".UTF-8";
            nullptr == std::setlocale(LC_ALL, locale) && FALSE == SetConsoleOutputCP(CP_UTF8)) {
            util::log_warn("Unable to set locale to {}", locale);
        }
#else
        if (constexpr auto* locale = "C.UTF-8"; nullptr == std::setlocale(LC_ALL, locale)) {
            util::log_warn("Unable to set locale to {}", locale);
        }
#endif
    }

#ifdef _WIN32
    void init_winsock()
    {
        constexpr auto version = MAKEWORD(2, 2);
        ::WSADATA data{};

        if (const auto error = ::WSAStartup(version, &data); error != 0) {
            util::log_critical("WSAStartup failed with error: {}", error);
            std::abort();
        }

        util::at_exit([] {
            ::WSACleanup();
        });
    }
#endif

    void init_filesystem()
    {
        auto& fs = common::FileSystemWrapper::get_instance();
        fs.load_or_abort();

        auto* const fs_interface = fs.get_interface_or_abort<common::FileSystemModuleInterface::filesystem>();
        fs_interface->mount();

        util::at_exit([fs_interface] {
            fs_interface->unmount();
        });
    }

    void init_engine(const char* const cmdline_args)
    {
        auto& engine = common::EngineWrapper::get_instance();
        engine.load_or_abort();

        auto& serverapi_interface = *engine.get_interface_or_abort<common::EngineInterface::server_api>();
        auto* const fs_interface_factory = common::FileSystemWrapper::get_instance().get_interface_factory();

        if (!serverapi_interface.init(".", cmdline_args, ::CreateInterface, fs_interface_factory)) {
            util::log_critical("Failed to initialize HLDS engine.");
            std::abort();
        }
    }

    void run_server_loop(std::shared_ptr<model::CmdLineArgs> cmdline_args, std::shared_ptr<abstract::View> view)
    {
        auto& engine = common::EngineWrapper::get_instance();
        auto* system_interface = engine.get_interface_system();
        auto& serverapi_interface = *engine.get_interface_or_abort<common::EngineInterface::server_api>();

        const auto server_loop = std::make_shared<model::ServerLoop>(std::move(cmdline_args));
        const auto console_commands = std::make_shared<model::ConsoleCommands>(system_interface);
        const auto input_history = std::make_shared<model::UserInputHistory>("input_history.txt");

        const auto input_presenter =
          std::make_shared<presenter::InputPresenter>(server_loop, input_history, console_commands);

        // cppcheck-suppress unreadVariable
        const auto output_presenter = std::make_unique<presenter::OutputPresenter>(server_loop, std::move(view));
        const auto console_input = std::make_unique<view::ConsoleInput>(input_presenter);

        server_loop->run(serverapi_interface);
        serverapi_interface.shutdown();
    }
}

int main(const int argc, const char** const argv)
{
    const auto con_debug = model::CmdLineArgs::find_param_index(argc, argv, "-condebug") != -1;
    const auto view = std::make_shared<view::ConsoleView>();
    constexpr auto* logfile = "qconsole.log";

#ifdef _WIN32
    util::log_init(con_debug, logfile, view);
#else
    const auto game_name = model::CmdLineArgs::get_param_option(argc, argv, "-game").value_or(std::string{});
    util::log_init(con_debug, game_name.empty() ? logfile : (game_name + "/" + logfile), view);
#endif

    const auto cmdline_args = std::make_shared<model::CmdLineArgs>(argc, argv);
    process_cmdline_args(*cmdline_args);
    init_locale();

#ifdef _WIN32
    init_winsock();
#endif

    init_filesystem();
    init_engine(cmdline_args->get_current());
    run_server_loop(cmdline_args, view);

    return 0;
}
