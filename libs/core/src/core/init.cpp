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

#include "core/init.hpp"
#include "common/engine/engine_wrapper.hpp"
#include "common/filesystem/filesystem_wrapper.hpp"
#include "util/lifecycle.hpp"
#include "util/logger.hpp"
#include <algorithm>
#include <clocale>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <WinSock2.h>
#endif

namespace Core {
    void init_logger(const CmdLineArgs& args, const std::shared_ptr<Util::LogOutput>& log_output)
    {
        constexpr auto* logfile = "qconsole.log";
        const auto enable_condebug = args.contains("-condebug");

#ifdef _WIN32
        Util::log_init(enable_condebug, logfile, log_output);
#else
        const auto game_name = args.get_argument_option("-game").value_or(std::string{});
        Util::log_init(enable_condebug, game_name.empty() ? logfile : (game_name + "/" + logfile), log_output);
#endif
    }

    void init_locale()
    {
#ifdef _WIN32
        constexpr auto* locale = ".UTF-8";

        if (nullptr == std::setlocale(LC_ALL, locale) && FALSE == SetConsoleOutputCP(CP_UTF8)) {
            Util::log_warn("Unable to set locale to {}", locale);
        }
#else
        constexpr auto* locale = "C.UTF-8";

        if (nullptr == std::setlocale(LC_ALL, locale)) {
            Util::log_warn("Unable to set locale to {}", locale);
        }
#endif
    }

    void init_socket()
    {
#ifdef _WIN32
        constexpr auto version = MAKEWORD(2, 2);
        ::WSADATA data{};

        if (const auto error = ::WSAStartup(version, &data); error != 0) {
            Util::log_critical("WSAStartup failed with error: {}", error);
            std::abort();
        }

        Util::at_exit([] {
            ::WSACleanup();
        });
#endif
    }

    void init_filesystem()
    {
        auto& fs = Common::FileSystemWrapper::get_instance();
        fs.load_or_abort();

        auto* const fs_interface = fs.get_interface_or_abort<Common::FileSystemModuleInterface::filesystem>();
        fs_interface->mount();

        Util::at_exit([fs_interface] {
            fs_interface->unmount();
        });
    }

    void init_engine(const CmdLineArgs& args)
    {
        auto& engine = Common::EngineWrapper::get_instance();
        engine.load_or_abort();

        auto& serverapi_interface = *engine.get_interface_or_abort<Common::EngineInterface::server_api>();
        auto* const fs_interface_factory = Common::FileSystemWrapper::get_instance().get_interface_factory();
        const auto* const cmdline_args_string = args.get_current();

        if (!serverapi_interface.init(".", cmdline_args_string, ::CreateInterface, fs_interface_factory)) {
            Util::log_critical("Failed to initialize HLDS engine.");
            std::abort();
        }
    }

    void init_server_loop(const CmdLineArgs& args, const std::shared_ptr<Model::ServerLoop>& server_loop)
    {
        if (const auto target_fps = args.get_argument_option_as<int>("-targetfps").value_or(0); target_fps > 0) {
            server_loop->set_target_fps(target_fps);
            server_loop->set_pingboost_level(Model::PingBoostLevel::target_fps);
        }
        else {
            constexpr auto min_value = static_cast<int>(Model::PingBoostLevel::not_set);
            constexpr auto max_value = static_cast<int>(Model::PingBoostLevel::level5);

            auto pingboost_value = args.get_argument_option_as<int>("-pingboost").value_or(0);
            pingboost_value = std::clamp(pingboost_value, min_value, max_value);

            server_loop->set_pingboost_level(static_cast<Model::PingBoostLevel>(pingboost_value));
        }
    }
}
